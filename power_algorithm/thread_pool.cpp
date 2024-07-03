#include "thread_pool.h"


thread_pool::thread_pool(int participants) : _size(participants)
{
	try {
		for (int i = 0; i < _size; i++)
		{
			pool.push_back(std::thread(&thread_pool::run_task, this));
		}
	}
	catch (...)
	{
		_done = true;
		throw;
	}
}
thread_pool::~thread_pool()
{
	_done = true;

	for (int i = 0; i < _size; i++)
	{
		pool[i].join();
	}
}

void thread_pool::run_task()
{
	while (!_done)
	{
		job task;
		if (tasks.try_pop(task))
		{
			task();
		}
		else
		{
			std::this_thread::yield();
		}
	}
}