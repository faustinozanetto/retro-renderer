#include "rtpch.h"
#include "job_system.h"

namespace retro::core {
	job_system::job_system()
	{
		m_thread_count = std::thread::hardware_concurrency();
		m_stop = false;
	}

	job_system::~job_system()
	{
		stop();
	}

	void job_system::start()
	{
		RT_TRACE("Retro Renderer | Job system starting.");
		for (int i = 0; i < m_thread_count; ++i)
		{
			RT_TRACE("Retro Renderer | Created Worker Thread #{}", i);
			m_threads.emplace_back(&job_system::setup_worker_thread, this);
		}
	}

	void job_system::stop()
	{
		RT_TRACE("Retro Renderer | Job system stopping.");
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_stop = true;
		}

		m_condition.notify_all();

		for (auto& thread : m_threads)
		{
			thread.join();
		}

		m_threads.clear();
	}

	void job_system::submit_job(const job_data& job)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_job_queue.push_back(job);
		m_condition.notify_one();
	}

	void job_system::wait_for_jobs(const std::vector<std::future<void>>& futures)
	{
		for (const auto& future : futures)
		{
			future.wait();
		}
	}

	void job_system::setup_worker_thread() {
		while (true)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_condition.wait(lock, [this] { return !m_job_queue.empty() || m_stop; });

			if (m_stop && m_job_queue.empty())
				break;

			job_data job = std::move(m_job_queue.front());
			m_job_queue.pop_front();

			lock.unlock();

			job.task();
			if (job.promise)
			{
				job.promise->set_value(); // Notify the completion of the job
			}
		}
	}
}