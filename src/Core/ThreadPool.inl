#include "ThreadPool.h"

ThreadPool::ThreadPool(const int& threadCount) 
                      : threadCount_(threadCount) 
{
    for(size_t i = 0; i < threadCount_; ++i) threads_.emplace_back(std::thread(&ThreadPool::work, this));
};

ThreadPool::~ThreadPool()
{ 
    active_ = false;
    signal_.notify_all();
    for(std::thread &thread : threads_) thread.join(); 
}

inline void ThreadPool::enqueueJob(std::packaged_task<void()>&& job)
{
    std::unique_lock<std::mutex> lock(mutex_);
    jobQueue_.emplace(std::move(job));
    signal_.notify_one();
}

inline void ThreadPool::waitForThreads()
{ 
    std::unique_lock<std::mutex> lock(mutex_);
    signal_.wait(lock, [this] { return jobQueue_.empty() && workingThreadCount_ == 0; });
}

inline size_t ThreadPool::getThreadCount() const { return threadCount_; }

void ThreadPool::work()
{
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
        --workingThreadCount_;

        if(jobQueue_.empty() && workingThreadCount_ == 0) {
            lock.lock();
            signal_.notify_all();
        }
    }
}

void rayTraceArea(Scene* scene, unsigned char image[], const int width, const int height, const int rowStart, const int rowEnd)
{
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
			image[idx]   = static_cast<unsigned char>(color.r);
			image[idx+1] = static_cast<unsigned char>(color.g);
			image[idx+2] = static_cast<unsigned char>(color.b);
		}
	}
}

inline void rayTrace(Scene* scene, unsigned char image[], const int& width, const int& height, ThreadPool& threadPool)
{
	int rowsToTrace = height / threadPool.getThreadCount();

	for(size_t i = 0; i < threadPool.getThreadCount(); ++i) {
		int rowStart = (int)i * rowsToTrace;
        int rowEnd = rowStart + rowsToTrace;

        // TODO: Profile to see whether std::bind or lambda is faster
		std::packaged_task<void()> job(std::bind(rayTraceArea, scene, image, width, height, rowStart, rowEnd));
        // std::packaged_task<void()> job([scene, image, width, height, rowStart, rowEnd]() {
        //     rayTraceArea(scene, image, width, height, rowStart, rowEnd);
        // });

		threadPool.enqueueJob(std::move(job));
	}

	threadPool.waitForThreads();
}