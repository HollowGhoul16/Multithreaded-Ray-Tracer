#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <vector>

#include "Renderer/Scene.h"

class ThreadPool {
public:
	ThreadPool(const int& threadCount);

	~ThreadPool();

	void enqueueJob(std::packaged_task<void()>&& job);

	void waitForThreads();

	size_t getThreadCount() const;

	void setThreadCount(const size_t newThreadCount);

private:
	std::atomic<size_t> threadCount_;
	std::vector<std::thread> threads_;
	std::atomic<int> workingThreadCount_{0};
	std::condition_variable signal_;
	std::queue<std::packaged_task<void()>> jobQueue_;
	std::mutex mutex_;
	bool active_ = true;

	void work();
};

void rayTraceArea(Scene* scene, unsigned char image[], const int rowStart, const int rowEnd, const int height, const int width);

void rayTrace(Scene* scene, unsigned char image[], const int& width, const int& height, ThreadPool& threadPool);

#include "ThreadPool.inl"