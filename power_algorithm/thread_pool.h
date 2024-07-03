#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <functional>

#include "thread_safe_queue.h"


class thread_pool
{
public:

	typedef std::function<void()> job;

	thread_pool(int participants);
	~thread_pool();


	template<typename Func>
	void submit(Func f)
	{
		tasks.emplace_back(f);
	}



private:

	void run_task();

	std::atomic<bool> _done;
	thread_safe_queue<job> tasks;

	std::vector<std::thread> pool;

	size_t _size = 0;
};