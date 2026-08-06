#ifndef __THREAD_POOL_HEADER__
#define __THREAD_POOL_HEADER__

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <string>
#include <functional>

class ThreadPool
{
public:
	ThreadPool(void);
	void								CreateThreads(size_t NumberOfThreads);
	void								SubmitJob(std::function<void()> NewJob);
	void								WaitInThread(int ThreadWaitDuration);
	void								CloseThreads(void);
	int									GetThreadNumber(void);
private:
	void								WorkerPool(void);
	bool								CloseThread;
	int									NumberThreads;

	std::mutex							ThreadMutex;
	std::vector<std::thread>			WorkerThread;
	std::queue<std::function<void()>>	JobThread;
	std::condition_variable				ThreadConditionVariable;
};

#endif