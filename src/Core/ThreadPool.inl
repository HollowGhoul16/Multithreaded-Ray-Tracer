#include "ThreadPool.h"

ThreadPool::ThreadPool(const int& threadCount) 
                      : threadCount_(threadCount) 
{
    for(size_t i = 0; i < threadCount_; ++i) threads_.emplace_back(std::thread(&ThreadPool::work, this));
}

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

inline void ThreadPool::setThreadCount(const size_t newThreadCount) { threadCount_ = newThreadCount; }

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

void rayTraceArea(Scene* scene, unsigned char imageData[], const int imageWidth, const int imageHeight, const int rowStart, const int rowEnd)
{
	for(int j = rowStart; j < rowEnd; ++j) {
		for (int i = 0; i < imageWidth; ++i)
		{
			// Translate pixels to world coords
            float x = (i + 0.5 - 0.5 * imageWidth);
            float y = (j + 0.5 - 0.5 * imageHeight);

			Color color = scene->getPixelColor(x, y);

			int idx = (j * imageWidth + i) * 3;
			imageData[idx]   = static_cast<unsigned char>(color.r);
			imageData[idx+1] = static_cast<unsigned char>(color.g);
			imageData[idx+2] = static_cast<unsigned char>(color.b);
		}
	}
}

inline void rayTrace(Scene* scene, unsigned char imageData[], const int& imageWidth, const int& imageHeight, ThreadPool& threadPool)
{
	int rowsToTrace = imageHeight / threadPool.getThreadCount();

	for(size_t i = 0; i < threadPool.getThreadCount(); ++i) {
		int rowStart = (int)i * rowsToTrace;
        int rowEnd = rowStart + rowsToTrace;

        // TODO: Profile to see whether std::bind or lambda is faster
		std::packaged_task<void()> job(std::bind(rayTraceArea, scene, imageData, imageWidth, imageHeight, rowStart, rowEnd));
        // std::packaged_task<void()> job([scene, image, width, height, rowStart, rowEnd]() {
        //     rayTraceArea(scene, image, width, height, rowStart, rowEnd);
        // });

		threadPool.enqueueJob(std::move(job));
	}

	threadPool.waitForThreads();
}