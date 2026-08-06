#include "ThreadPool.h"
#include "../Common/Console.h"

ThreadPool::ThreadPool(void)
{
	this->NumberThreads = 0;
	this->CloseThread = false;
}

int ThreadPool::GetThreadNumber(void) { return this->NumberThreads; }
void ThreadPool::WaitInThread(int ThreadWaitDuration) { std::this_thread::sleep_for(std::chrono::milliseconds(ThreadWaitDuration)); }

void ThreadPool::CreateThreads(size_t NumberOfThreads)
{
	Console::SysPrint("[CreateThreads]: Number of threads allocated: " + std::to_string(NumberOfThreads), 1);
	for (size_t Index = 0; Index < NumberOfThreads; Index++)
	{
		this->WorkerThread.emplace_back([this]() {this->WorkerPool(); });
	}
	this->NumberThreads += static_cast<int>(NumberOfThreads);
}

void ThreadPool::SubmitJob(std::function<void()> NewJob)
{
	{
		std::unique_lock<std::mutex> MutexLock(this->ThreadMutex);
		this->JobThread.emplace(NewJob);
	}
	this->ThreadConditionVariable.notify_one();
}

void ThreadPool::WorkerPool(void)
{
	while (!this->CloseThread)
	{
		std::function<void()> JobWork;
		{
			std::unique_lock<std::mutex> MutexLock(this->ThreadMutex);
			this->ThreadConditionVariable.wait(MutexLock, [this]() {return !this->JobThread.empty() || this->CloseThread; });

			if (this->CloseThread) { return; }
			JobWork = this->JobThread.front();
			this->JobThread.pop();
		}
		JobWork();
	}
}

void ThreadPool::CloseThreads(void)
{
	Console::SysPrint("[CloseThreads]: Shutting down threads.", 1);

	this->NumberThreads = 0;
	this->CloseThread = true;
	this->ThreadConditionVariable.notify_all();

	for (auto& Thread : this->WorkerThread) { Thread.join(); }
}