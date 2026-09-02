#include "scheduler.h"

void core::Scheduler::Schedule(Callback cb, std::chrono::milliseconds interval)
{
	std::lock_guard<std::mutex> lock(mtx_);
	tasks_.push(
		{ std::chrono::steady_clock::now() + interval, interval, std::move(cb)}
	);
	cv_.notify_one();
}

void core::Scheduler::Run()
{
	std::unique_lock<std::mutex> lock(mtx_);
	while (running_) {
		if (tasks_.empty()) {
			cv_.wait(lock);
			continue;
		}

		auto next = tasks_.top().next_run;
		if(cv_.wait_until(lock, next) == std::cv_status::timeout) {
			Task task = tasks_.top();
			tasks_.pop();
			lock.unlock();
			task.callback();
			lock.lock();
			task.next_run = std::chrono::steady_clock::now() + task.interval;
			tasks_.push(task);
		}
	}
}

void core::Scheduler::Stop()
{
	{
		std::lock_guard<std::mutex> lock(mtx_);
		running_ = false;
	}
	cv_.notify_one();
}
