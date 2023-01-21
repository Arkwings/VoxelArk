#include "common.hpp"

ThreadPool::ThreadPool() {
    const uint32_t num_threads = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 3; // Max # of threads the system supports
    std::cout << "max concurrent threads: " << num_threads << std::endl;
    threads.resize(num_threads);
    for (uint32_t i = 0; i < num_threads; i++) {
        threads[i] = std::thread(&ThreadPool::ThreadLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

void ThreadPool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::busy() {
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::ThreadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
                });
            if (should_terminate) return;
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}



OGLThreadPool::OGLThreadPool() {
    const uint32_t num_threads = 1;
    threads.resize(num_threads);
    for (uint32_t i = 0; i < num_threads; i++) {
        threads[i] = std::thread(&OGLThreadPool::ThreadLoop, this);
    }
}

OGLThreadPool::~OGLThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

void OGLThreadPool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

bool OGLThreadPool::IsIdle() {
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = jobs.empty();
    }
    return poolbusy && is_idle;
}

void OGLThreadPool::ThreadLoop() {
    glfwMakeContextCurrent(G_Window->GetWindow());

    //glad
    if (!gladLoadGL(glfwGetProcAddress)) throw std::runtime_error("Failed to initialize OpenGL context");

    //gl
    GLint NumberOfExtensions;
    glGetIntegerv(static_cast<unsigned int>(GL_NUM_EXTENSIONS), &NumberOfExtensions);
    for (int i = 0; i < NumberOfExtensions; i++) {
        const char* ccc = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
        std::cout << ccc << " supported" << std::endl;
    }
    DEBUG_NAME_PRINT("active CG vendor: ", glGetString(GL_VENDOR));
    DEBUG_NAME_PRINT("active CG model: ", glGetString(GL_RENDERER));

    glViewport(0, 0, G_Window->GetWidth(), G_Window->GetHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
#endif
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    // glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &TEX::MAX_TEX_SIZE);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &TEX::MAX_TEX_MAX_ANISOTROPY);

    while (true) {

        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            is_idle = true;
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
                });
            is_idle = false;
            if (should_terminate) return;
            job = jobs.front();
            jobs.pop();
        }
        job();
    }

    glfwDestroyWindow(G_Window->GetWindow());
}
