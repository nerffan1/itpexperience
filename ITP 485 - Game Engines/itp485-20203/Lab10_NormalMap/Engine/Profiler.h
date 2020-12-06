#pragma once
#include "stdafx.h"

#define PROFILE_SCOPE(name) \
Profiler::ScopedTimer name##_scope(Profiler::Get()->GetTimer(std::string(#name)))

class Profiler
{
public:

	//Timer Class
	class Timer
	{
	public:
		Timer(std::string pName);
		~Timer();
		void Start();
		void Stop();
		void Reset();
		const std::string& GetName() const { return mName; }
		double GetTime_ms() const { return mRecentFrame; }
		double GetMax_ms() const { return mLongestFrame; }
		double GetAvg_ms() const { return mTotalTime / mFrames; }

	private:
		friend class Profiler;

		//Variables
		std::chrono::high_resolution_clock::time_point mStart;
		double mRecentFrame;
		double mLongestFrame;
		double mTotalTime;
		int mFrames;
		std::string mName;
	};

	//ScopedTimer
	class ScopedTimer
	{
	public:
		ScopedTimer(Timer* pTimer);
		~ScopedTimer();
	private:
		Profiler::Timer* mTimer;
	};

	//Profiler Functions
	static Profiler* Get() { return &sProf; }
	Timer* GetTimer(const std::string& name);
	void ResetAll();

private:
	Profiler();
	~Profiler();
	//Static Members
	static Profiler sProf;
	bool firstWrite = true;

	//Variables 
	std::unordered_map<std::string, Profiler::Timer*> mTimers;
	FILE* scopeFile;
};
