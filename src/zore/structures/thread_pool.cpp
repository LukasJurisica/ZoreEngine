#include "zore/structures/thread_pool.hpp"
#include <algorithm>

namespace zore {

	//========================================================================
	//	Threadpool
	//=========================================================================

	thread_pool::thread_pool(uint32_t num_threads) {
		num_threads = std::min(std::max(num_threads, 1u), get_max_thread_count());
		m_threads.reserve(num_threads);
		for (uint32_t i = 0; i < num_threads; i++)
			m_threads.emplace_back(&thread_pool::thread_loop, this);
	}

	thread_pool::~thread_pool() {
		m_job_mutex.lock();
		m_running = false;
		m_job_mutex.unlock();
		cv.notify_all();
		for (std::thread& worker : m_threads)
			worker.join();
		for (Job* job : m_jobs)
			delete job;
		for (auto& [type_index, handler] : m_callback_handlers)
			delete handler;
	}

	void thread_pool::update_priorities() {
		std::lock_guard<std::mutex> lock(m_job_mutex);
		for (Job* job : m_jobs)
			job->update_priority();
		std::sort(m_jobs.begin(), m_jobs.end(), [](Job* a, Job* b) { return a->get_priority() > b->get_priority(); });
	}

	void thread_pool::thread_loop() {
		while (m_running) {
			Job* job = pop();
			if (!job)
				continue;
			job->execute();

			auto iter = m_callback_handlers.find(typeid(*job));
			if (iter != m_callback_handlers.end())
				iter->second->execute(*job);
			delete job;
		}
	}

	Job* thread_pool::pop() {
		std::unique_lock<std::mutex> lock(m_job_mutex);
		cv.wait(lock, [&] { return !m_jobs.empty() || !m_running; });

		Job* job = nullptr;
		if (!m_jobs.empty()) {
			job = m_jobs.front();
			m_jobs.erase(m_jobs.begin());
		}
		return job;
	}
}