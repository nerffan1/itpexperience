	#pragma once
#include "Worker.h"
#include "job.h"
#include <queue>
#include <atomic>

class JobManager
{
public:
	void Begin();
	void End();
	void AddJob(class Job* pJob);
	void WaitForJobs();
	void ShutWorkers() { Worker::sShutDown = true; }

	//Static members Variables
	static JobManager* GetMan() { return &sMan; };
	static std::queue<Job*> mJobs;

private:
	//Friend Classes
	friend class Worker;
	
	//Member Variables
	Worker mWorker[4];
	short int workerSize = 4;
	
	//Static Members
	static JobManager sMan;
	static std::atomic<int> mJobCount;
};