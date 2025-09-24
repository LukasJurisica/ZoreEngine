#include "zore/structures/ThreadPool.hpp"

#include "zore/math/MathUtils.hpp"

#include <algorithm>

namespace zore {

	//========================================================================
	//	Threadpool Class
	//=========================================================================

	ThreadPool::ThreadPool(uint32_t num_threads) {
		num_threads = zm::Min(zm::Max(num_threads, 1u), GetMaximumThreadCount());
		m_threads.reserve(num_threads);
		for (uint32_t i = 0; i < num_threads; i++)
			m_threads.emplace_back(&ThreadPool::ThreadLoop, this);
	}

	ThreadPool::~ThreadPool() {
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

	void ThreadPool::UpdatePriorities() {
		std::lock_guard<std::mutex> lock(m_job_mutex);
		for (Job* job : m_jobs)
			job->UpdatePriority();
		std::sort(m_jobs.begin(), m_jobs.end(), [](Job* a, Job* b) { return a->GetPriority() > b->GetPriority(); });
	}

	void ThreadPool::ThreadLoop() {
		while (m_running) {
			Job* job = GetNextJob();
			if (!job)
				continue;
			job->Execute();

			auto iter = m_callback_handlers.find(typeid(*job));
			if (iter != m_callback_handlers.end())
				iter->second->Execute(*job);
			delete job;
		}
	}

	Job* ThreadPool::GetNextJob() {
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