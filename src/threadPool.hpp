#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <vector>

#include "scene.hpp"

class ThreadPool {
public:
	ThreadPool(const int& threadCount) : threadCount_(threadCount) {
		for(size_t i = 0; i < threadCount_; ++i) threads_.emplace_back(std::thread(&ThreadPool::work, this));
	};

	~ThreadPool() { 
		active_ = false;
		signal_.notify_all();
		for(std::thread &thread : threads_) thread.join(); 
	}

	void enqueueJob(std::packaged_task<void()>&& job) {
		std::unique_lock<std::mutex> lock(mutex_);
		jobQueue_.emplace(std::move(job));
		signal_.notify_one();
	}

	void waitForThreads() { 
		std::unique_lock<std::mutex> lock(mutex_);
		signal_.wait(lock, [this] { return jobQueue_.empty() && workingThreadCount_ == 0; });
	}

	size_t getThreadCount() const { return threadCount_; }

private:
	const size_t threadCount_;
	std::vector<std::thread> threads_;
	std::condition_variable signal_;
	std::queue<std::packaged_task<void()>> jobQueue_;
	std::mutex mutex_;
	bool active_ = true;
	int workingThreadCount_ = 0;

	void work() {
		while(active_) {
			thread_local std::packaged_task<void()> job;

			std::unique_lock<std::mutex> lock(mutex_);
			signal_.wait(lock, [this] { return !jobQueue_.empty() || !active_; });
			if(!active_) break;
			++workingThreadCount_;
			job.swap(jobQueue_.front());
			jobQueue_.pop();
			lock.unlock();

			job();

			lock.lock();
			--workingThreadCount_;
			lock.unlock();

			if(jobQueue_.empty() && workingThreadCount_ == 0) {
				lock.lock();
				signal_.notify_all();
			}
		}
	}
};

void rayTraceArea(Scene* scene, unsigned char image[], int rowStart, int rowEnd, int height, int width) {
	for(int i = rowStart; i < rowEnd; i++) {
		for (int j = 0; j < width; j++)
		{
			// Translate pixels to image plane coords (normalize each pixel)
			float u = (j + 0.5) / width;
			float v = (i + 0.5) / height;

			// Translate image plane coords to world coords
			float x = (u - 0.5) * width;
			float y = (0.5 - v) * height;

			Color color = scene->getPixelColor(x, y);
			
			int idx = (i * width + j) * 3;
			image[idx] = (unsigned char)(color.r);
			image[idx+1] = color.g;
			image[idx+2] = color.b;
		}
	}
}

void rayTrace(Scene* scene, unsigned char image[], int width, int height, ThreadPool& threadPool) {
	int rowsToTrace = height / threadPool.getThreadCount();

	for(size_t i = 0; i < threadPool.getThreadCount(); ++i) {
		int temp = (int)i * rowsToTrace;
		std::packaged_task<void()> job(std::bind(rayTraceArea, scene, image, temp, temp + rowsToTrace, height, width));
		threadPool.enqueueJob(std::move(job));
	}

	threadPool.waitForThreads();
}