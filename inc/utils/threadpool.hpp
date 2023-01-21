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

class OGLThreadPool {
    public:
    OGLThreadPool();
    ~OGLThreadPool();
    OGLThreadPool(ThreadPool&) = delete;
    OGLThreadPool& operator=(const OGLThreadPool&) = delete;

    void QueueJob(const std::function<void()>& job);
    bool IsIdle();
    void ThreadLoop();

    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
    bool should_terminate = false;           // Tells threads to stop looking for jobs
    bool is_idle = true;
};

inline ThreadPool* G_ThreadPool = nullptr;
inline OGLThreadPool* G_OGLThreadPool = nullptr;