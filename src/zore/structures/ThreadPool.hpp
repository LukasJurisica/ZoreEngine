#pragma once

#include "zore/utils/DataTypes.hpp"

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <typeindex>
#include <functional>

namespace zore {

	//========================================================================
	//	Job Class
	//========================================================================

	class Job {
	public:
		Job(int priority = 0) : m_priority(priority) {}
		virtual ~Job() = default;

		virtual void Execute() = 0;
		virtual void UpdatePriority() {}
		void SetPriority(int priority) { m_priority = priority; }
		int GetPriority() const { return m_priority; }

	private:
		int m_priority;
	};

	//========================================================================
	//	Job Callback Handler Class
	//========================================================================

	template<typename JobType>
	using JobCallback = std::function<void(const JobType&)>;

	class JobCallbackHandlerBase {
	public:
		virtual ~JobCallbackHandlerBase() = default;
		virtual void Execute(const Job& e) const = 0;
	};

	template <typename JobType>
	class JobCallbackHandler : public JobCallbackHandlerBase {
	public:
		explicit JobCallbackHandler(JobCallback<JobType> callback) : m_callback(callback) {}
		JobCallbackHandler(const JobCallbackHandler&) = delete;
		JobCallbackHandler& operator=(const JobCallbackHandler&) = delete;
		~JobCallbackHandler() override = default;

		inline void Execute(const Job& e) const override { m_callback(static_cast<const JobType&>(e)); }

	private:
		JobCallback<JobType> m_callback;
	};

	//========================================================================
	//	Threadpool Class
	//========================================================================

	class ThreadPool {
	public:
		ThreadPool(uint32_t num_threads);
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		~ThreadPool();

		static uint32_t GetMaximumThreadCount() { return std::thread::hardware_concurrency(); };
		void UpdatePriorities();

		template <typename JobType>
		void RegisterCallback(void(*callback)(const JobType&)) {
			RegisterCallback<JobType>(JobCallback<JobType>(std::bind(callback, std::placeholders::_1)));
		}

		template <typename ClassType, typename JobType>
		void RegisterCallback(void(ClassType::*callback)(const JobType&), ClassType* obj) {
			RegisterCallback<JobType>(JobCallback<JobType>(std::bind(callback, obj, std::placeholders::_1)));
		}

		template <typename JobType>
			requires(std::is_base_of_v<Job, JobType>&& std::is_copy_constructible_v<JobType>)
		void RegisterCallback(std::function<void(const JobType&)> callback) {
			std::lock_guard<std::mutex> lock(m_job_mutex);
			m_callback_handlers[typeid(JobType)] = new JobCallbackHandler<JobType>(callback);
		}

		template <typename JobType>
			requires(std::is_base_of_v<Job, JobType> && std::is_copy_constructible_v<JobType>)
		void Enqueue(const JobType& job) {
			std::lock_guard<std::mutex> lock(m_job_mutex);
			m_jobs.push_back(new JobType(job));
			cv.notify_one();
		}

	private:
		void ThreadLoop();
		Job* GetNextJob();

	private:
		std::vector<std::thread> m_threads;
		std::vector<Job*> m_jobs;
		std::unordered_map<std::type_index, JobCallbackHandlerBase*> m_callback_handlers;
		std::mutex m_job_mutex;
		std::condition_variable cv;
		bool m_running = true;
	};
}