#pragma once

class Scheduler {
    public:
    Scheduler(const double& delta_time_between_execs);
    ~Scheduler();
    Scheduler(Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    void CheckTime();
    void ExecuteJob();
    void QueueJob(const size_t& hash, const std::function<void()>& job);
    const bool IsEmpty();

    private:
    std::deque<std::pair<size_t, std::function<void()>>> jobs_;
    double next_exec_time_;
    const double delta_time_between_execs_;
};