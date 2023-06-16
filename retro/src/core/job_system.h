#pragma once

#include "utils/singleton.h"

#include <functional>
#include <future>
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace retro::core {
	struct job_data {
		std::function<void()> task;
		std::shared_ptr<std::promise<void>> promise; // Updated to shared_ptr

		job_data() = default;
		job_data(const job_data&) = default;
		job_data(job_data&&) = default;
		job_data& operator=(const job_data&) = default;
		job_data& operator=(job_data&&) = default;
    };

    class job_system : public singleton<job_system> {
    public:
        job_system();
        ~job_system();

		job_system(const job_system&) = delete;
		job_system& operator=(const job_system&) = delete;

        /* Getters */
        std::mutex& get_mutex() { return m_mutex; }

        void start();
        void stop();

        void submit_job(const job_data& job);
        void wait_for_jobs(const std::vector<std::future<void>>& futures);

    private:
        void setup_worker_thread();
    private:
		unsigned int m_thread_count;
		bool  m_stop;
		std::mutex  m_mutex;
		std::condition_variable m_condition;
		std::vector<std::thread> m_threads;
		std::deque<job_data> m_job_queue;
    };
}