#pragma once

#include "zore/utils/sized_integer.hpp"
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

		virtual void execute() = 0;
		virtual void update_priority() {}
		void set_priority(int priority) { m_priority = priority; }
		int get_priority() const { return m_priority; }

	private:
		int m_priority;
	};

	//========================================================================
	//	Job Callback Handler Class
	//========================================================================

	template<typename T>
	using job_callback = std::function<void(const T&)>;

	class job_callback_handler_base {
	public:
		virtual ~job_callback_handler_base() = default;
		virtual void execute(const Job& e) const = 0;
	};

	template <typename T>
	class job_callback_handler : public job_callback_handler_base {
	public:
		explicit job_callback_handler(job_callback<T> callback) : m_callback(callback) {}
		job_callback_handler(const job_callback_handler&) = delete;
		job_callback_handler& operator=(const job_callback_handler&) = delete;
		~job_callback_handler() override = default;

		inline void execute(const Job& e) const override { m_callback(static_cast<const T&>(e)); }

	private:
		job_callback<T> m_callback;
	};

	//========================================================================
	//	Threadpool Class
	//========================================================================

	class thread_pool {
	public:
		thread_pool(uint32_t num_threads);
		thread_pool(const thread_pool&) = delete;
		thread_pool& operator=(const thread_pool&) = delete;
		~thread_pool();

		static uint32_t get_max_thread_count() { return std::thread::hardware_concurrency(); };
		void update_priorities();

		template <typename T>
		void register_callback(void(*callback)(const T&)) {
			register_callback<T>(job_callback<T>(std::bind(callback, std::placeholders::_1)));
		}

		template <typename C, typename T>
		void register_callback(void(C::*callback)(const T&), C* obj) {
			register_callback<T>(job_callback<T>(std::bind(callback, obj, std::placeholders::_1)));
		}

		template <typename T>
			requires(std::is_base_of_v<Job, T>&& std::is_copy_constructible_v<T>)
		void register_callback(std::function<void(const T&)> callback) {
			std::lock_guard<std::mutex> lock(m_job_mutex);
			if (m_callback_handlers.find(typeid(T)) != m_callback_handlers.end())
				delete m_callback_handlers[typeid(T)];
			m_callback_handlers[typeid(T)] = new job_callback_handler<T>(callback);
		}

		template <typename T>
			requires(std::is_base_of_v<Job, T>&& std::is_copy_constructible_v<T>)
		void enqueue(const T& job) {
			std::lock_guard<std::mutex> lock(m_job_mutex);
			m_jobs.push_back(new T(job));
			cv.notify_one();
		}

	private:
		void thread_loop();
		Job* pop();

	private:
		std::vector<std::thread> m_threads;
		std::vector<Job*> m_jobs;
		std::unordered_map<std::type_index, job_callback_handler_base*> m_callback_handlers;
		std::mutex m_job_mutex;
		std::condition_variable cv;
		bool m_running = true;
	};
}