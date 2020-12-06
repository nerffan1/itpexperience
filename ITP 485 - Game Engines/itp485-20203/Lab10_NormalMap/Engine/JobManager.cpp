#include "stdafx.h"
#include "JobManager.h"

bool Worker::sShutDown{ false };
std::queue<Job*> JobManager::mJobs;
JobManager JobManager::sMan = JobManager();
std::atomic<int> JobManager::mJobCount = 0;

void JobManager::Begin()
{
	for (auto i = 0; i < workerSize; i++)
	{
		mWorker[i].Begin();
	}
}

void JobManager::End()
{
	ShutWorkers();
	for (auto i = 0; i < workerSize; i++)
	{
		mWorker[i].End();
	}
}

void JobManager::AddJob(Job* pJob)
{
	mJobs.push(pJob);
	++mJobCount;
}

void JobManager::WaitForJobs()
{
	while (mJobCount != 0)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}