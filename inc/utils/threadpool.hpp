#pragma once

class ThreadPool {
    public:
    ThreadPool();
    ~ThreadPool();
    ThreadPool(ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void QueueJob(const std::function<void()>& job);
    bool busy();
    void ThreadLoop();

    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
    bool should_terminate = false;           // Tells threads to stop looking for jobs
};

inline ThreadPool* G_ThreadPool = new ThreadPool();