#pragma once
#include <thread>
#include "stdafx.h"
#include <mutex>

class Worker
{
public:
	void Begin();
	void End();
	static void Loop();
	static bool sShutDown;
private:
	std::thread* mthread;
};