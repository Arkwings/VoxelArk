#include "common.hpp"

Scheduler::Scheduler(const double& delta_time_between_execs)
    : delta_time_between_execs_(delta_time_between_execs)
    , next_exec_time_(std::chrono::duration<double>((std::chrono::high_resolution_clock::now()).time_since_epoch()).count() + delta_time_between_execs) {

}

Scheduler::~Scheduler() {}

void Scheduler::CheckTime() {
    if (std::chrono::duration<double>((std::chrono::high_resolution_clock::now()).time_since_epoch()).count() > next_exec_time_) {
        ExecuteJob();
        next_exec_time_ += delta_time_between_execs_;
    }
}

void Scheduler::ExecuteJob() {
    if (!IsEmpty()) {
        jobs_[0].second();
        jobs_.pop_front();
    }
}

void Scheduler::QueueJob(const size_t& hash, const std::function<void()>& job) {
    bool found(false);

#pragma omp parallel for
    for (int i = 0; i < jobs_.size(); ++i) {
        if (jobs_[i].first == hash) {
            found = true;
            break;
        }
    }
    if (!found)jobs_.push_back({ hash, job });
}

const bool Scheduler::IsEmpty() {
    return jobs_.empty();
}
