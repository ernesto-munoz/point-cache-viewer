#pragma once

#include <chrono>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>

namespace core {
	using Callback = std::function<void()>;

	struct Task {
		std::chrono::steady_clock::time_point next_run;
		std::chrono::milliseconds interval;
		Callback callback;

		bool operator>(const Task& other) const {
			return next_run > other.next_run;
		}
	};

	class Scheduler {
	private:
		std::priority_queue<Task, std::vector<Task>, std::greater<>> tasks_;  // priority queue (min priority == min time first)
		std::mutex mtx_;
		std::condition_variable cv_;
		bool running_ = true;
		static inline std::shared_ptr<Scheduler> instance_ptr_;
		static inline std::thread runner_;
	public:
		void Schedule(Callback cb, std::chrono::milliseconds interval);
		void Run();
		void Stop();

		static std::shared_ptr<Scheduler> Instance() {
			static std::shared_ptr<Scheduler> instance = [] {
				instance_ptr_ = std::make_shared<Scheduler>();
				// run the thread with the scheduler and save the thread 
				runner_ = std::thread([]() { instance_ptr_->Run(); });
				// need shutdown the scheduler and thread at exit!
				std::atexit(Scheduler::Shutdown);
				return instance_ptr_;
			}();
			return instance;
		}

		static void Shutdown() {
			if (instance_ptr_) {
				instance_ptr_->Stop();
			}
			if (runner_.joinable()) {
				runner_.join();
			}
		}
	};
}
