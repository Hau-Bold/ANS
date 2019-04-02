#ifndef THREAD_POOL_EXECUTOR_H
#define THREAD_POOL_EXECUTOR_H

#include<condition_variable>
#include<queue>
#include<future>
#include <functional>
#include<thread>
#include<vector>>
#include"ANSTypes.h"

using namespace cogra;

using Task = std::function<void()>;

/**the class ThreadPoolExecutor, not used at moment*/
class ThreadPool
{
private:

	std::vector<std::thread>m_Threads;

	/**to hold the tasks*/
	std::queue<Task> mTasks;

	/**waits until stopping is set*/
	std::condition_variable m_ConditionVariable;
	std::mutex m_Mutex;
	bool stopping = false;

	void stop() noexcept
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			stopping = true;
		}
	
		m_ConditionVariable.notify_all();
	
		for (auto & t : m_Threads)
		{
			t.join();
		}
	}

	 void start(uint32 countOfThreads)
		{
			for (auto i=0;i<countOfThreads;i++)
			{
		
				m_Threads.emplace_back([&]
				{
		
					while (true)
					{
						Task task;
						{
							/**Freigabe nach Verlassen des kritischen Bereichs*/
							std::unique_lock<std::mutex> lock(m_Mutex);
		
							m_ConditionVariable.wait(lock, [=] {return stopping || !mTasks.empty(); });
		
							if (stopping && mTasks.empty())
							{
								break;
							}
		
							task = std::move(mTasks.front());
							mTasks.pop();
						}
		
						task();
					}
				}
				);
		
			}
		}

	


public:
	/**
	*Constructor.
	*
	*@param countOfThreads - the count of tasks to process parallel 
	*/
	ThreadPool(uint32 countOfThreads)
	{
		start(countOfThreads);
	}

	template<class T>
	auto enqueue(T task)->std::future<decltype(task())>
	{

		auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));

		{
			std::unique_lock<std::mutex> lock{ m_Mutex };
			mTasks.emplace([=] {
				(*wrapper)();
			});
		}
		m_ConditionVariable.notify_one();


		return wrapper->get_future();

	}

	/**
	*Destructor.
	*/
	~ThreadPool()
	{
		stop();
	}



	void waitForAll()
	{
		for (auto &thread : m_Threads)
		{
			thread.join();
		}
	}
};
#endif