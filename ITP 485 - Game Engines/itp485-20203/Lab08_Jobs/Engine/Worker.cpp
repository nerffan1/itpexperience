#include "stdafx.h"
#include "Worker.h"
#include "JobManager.h"
#include "Job.h"

std::mutex mMutex;

void Worker::Begin()
{
	mthread = new std::thread(Loop);
}

void Worker::End()
{
	mthread->join();
	delete mthread;
}

void Worker::Loop()
{
	Job* newJob = nullptr;
	while (!sShutDown)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		while (JobManager::mJobCount > 0)
		{
			mMutex.lock();
			if (!JobManager::mJobs.empty())
			{
				newJob = JobManager::mJobs.front();
				JobManager::mJobs.pop();
			}
			mMutex.unlock();
			if (newJob != nullptr)
			{
				--JobManager::mJobCount; //Assuming DoIt variables aren't affected immediatly after
				newJob->DoIt();
				newJob = nullptr;
			}
		}

	}
}
