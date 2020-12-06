#pragma once
#include <exception>

class Nonfatal : public virtual std::exception
{
public:
	Nonfatal(int p) : mCode(p) {};
		const char* what() const noexcept override { return "MY NON-FATAL EXCEPTION!"; }
	int mCode;
};

class Fatal : public virtual  std::exception
{
public:
	Fatal(int p) : mCode(p) {};
	const char* what() const noexcept override { return "MY FATAL EXCEPTION!"; }
	int mCode;
};