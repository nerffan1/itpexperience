#include "stdafx.h"
#include "Profiler.h"

Profiler Profiler::sProf = Profiler();

Profiler::Timer::Timer(std::string pName) : mRecentFrame(0.0),
mLongestFrame(0.0),
mTotalTime(0.0),
mFrames(0),
mName(pName)
{

}

Profiler::Timer::~Timer()
{
	Stop();
}

void Profiler::Timer::Start()
{
	mStart = std::chrono::high_resolution_clock::now();
	if (!Profiler::Get()->firstWrite)
	{
		fprintf(Profiler::Get()->scopeFile, ",");
	}
	else
	{
		Profiler::Get()->firstWrite = false;
	}
	fprintf(Profiler::Get()->scopeFile, "\n{ \"name\": \"%s\", ", mName.c_str());
	fprintf(Profiler::Get()->scopeFile, "\"cat\": \"%s\", ", mName.c_str());
	fprintf(Profiler::Get()->scopeFile, "\"ph\": \"B\", \"ts\": ");
	//ts : Tracing Clock timestamp of the event in 
	fprintf(Profiler::Get()->scopeFile, "%f", (double)mStart.time_since_epoch().count() / 1000);
	//pid : Process ID for the process that output this event
	fprintf(Profiler::Get()->scopeFile, ", \"pid\": 1");
	//tid : Thread ID for the thread that output this event.
	fprintf(Profiler::Get()->scopeFile, ", \"tid\": 1}");
}

void Profiler::Timer::Stop()
{
	//Update Timer data
	auto end = std::chrono::high_resolution_clock::now();
	double nano = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - mStart).count();
	mRecentFrame += nano / 1000000;

	//Write to JSON File
	fprintf(Profiler::Get()->scopeFile, ",{ \"name\": \"%s\", ", mName.c_str());
	fprintf(Profiler::Get()->scopeFile, "\"cat\": \"%s\", ", mName.c_str());
	fprintf(Profiler::Get()->scopeFile, "\"ph\": \"E\", \"ts\": ");
	//ts : Tracing Clock timestamp of the event in 
	fprintf(Profiler::Get()->scopeFile, "%f", (double)end.time_since_epoch().count() / 1000);
	//pid : Process ID for the thread that output this event
	fprintf(Profiler::Get()->scopeFile, ", \"pid\": 1");
	//tid : Thread ID for the thread that output this event.
	fprintf(Profiler::Get()->scopeFile, ", \"tid\": 1 }");
}

void Profiler::Timer::Reset()
{
	mTotalTime += mRecentFrame;
	++mFrames;
	if (mLongestFrame < mRecentFrame) mLongestFrame = mRecentFrame;
	mRecentFrame = 0.0;
}

Profiler::Timer* Profiler::GetTimer(const std::string& name)
{
	if (mTimers.find(name) != mTimers.end())
	{
		return mTimers[name];
	}
	mTimers[name] = new Profiler::Timer(name);
	return mTimers[name];
}

void Profiler::ResetAll()
{
	for (auto timer : mTimers)
	{
		timer.second->Reset();
	}
}

Profiler::Profiler()
{
	fopen_s(&scopeFile, "profile.json", "w");
	fprintf(scopeFile, "[");
}

Profiler::~Profiler()
{
	FILE* proFile;
	fopen_s(&proFile, "profile.txt", "w");

	//Write top information 
	fprintf(proFile, " name :  avg (ms),  max  (ms) \n");

	//Write Max and 
	for (auto timer : mTimers)
	{
		fprintf(proFile, "%s %s %lf %s %lf", timer.first.c_str(), ":  ", timer.second->GetAvg_ms(), "  ", timer.second->GetMax_ms());
		fprintf(proFile, "\n");
	}
	fclose(proFile);

	//Write end of json file
	fprintf(scopeFile, "]");
	fclose(scopeFile);
}

Profiler::ScopedTimer::ScopedTimer(Timer* pTimer) : mTimer(pTimer)
{
	mTimer->Start();
}

Profiler::ScopedTimer::~ScopedTimer()
{
	mTimer->Stop();
}

