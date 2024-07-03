#pragma once


template <typename T>
class thread_safe_queue
{
public:

	thread_safe_queue() {}
	~thread_safe_queue() {}

	void emplace_back(T&& t_)
	{
		{
			std::lock_guard<std::mutex> lck(mtx);
			data_queue.emplace_back(t_);
		}

		_cv.notify_all();
	}

	bool try_pop(T& value_)
	{
		std::lock_guard<std::mutex> lck(mtx);
		if (data_queue.empty())
		{
			return false;
		}

		value_ = data_queue.front();
		data_queue.pop_front();
		return true;
	}

	void wait_and_pop(T& value_)
	{
		std::unique_lock<std::mutex> lck(mtx);

		_cv.wait(lck, []()
			{
				return !data_queue.empty();
			});

		value_ = data_queue.front();
		data_queue.pop_front();
	}

	bool empty()
	{
		std::lock_guard < std::mutex> lk(mtx);
		return data_queue.empty();
	}

private:
	std::mutex mtx;
	std::condition_variable _cv;
	std::deque<T> data_queue;
};