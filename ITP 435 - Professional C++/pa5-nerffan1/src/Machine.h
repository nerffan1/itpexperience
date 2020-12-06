#pragma once
#include "Ops.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include <fstream>
#include "../img/bitmap_image.hpp"

class Machine
{
public:
	Machine();
	Machine(const Machine&) = delete;
	Machine(const Machine&&) = delete;
	Machine& operator= (const Machine&) = delete;
	Machine& operator= (const Machine&&) = delete;
	~Machine() { PrintInfo(); mLog.close(); }

	//Create Op Function
	template <typename T>
	static std::shared_ptr<Op> CreateOp() { return std::make_shared<T>(); }

	//Member Functions
	void ReadAseembly(std::string pFile);
	void Execute();

	//Turtle
	void Turtle(std::string reg1, bool pos = true);

	//Printing
	void PrintInfo();
	void PrintStack();

	//Get/Set Registers
	int GetRegister(std::string p) { return mRegisters[p]; }
	void SetRegister(std::string p, int value);
	void popPushReg(std::string p, int value) noexcept;

	//Get/Set Flags
	bool GetFlag(std::string p) { return mFlags[p]; }
	void SetFlag(std::string p, bool value) { mFlags[p] = value; }

	//Push/Pop Stack
	void pushStack(int& pIn);
	void popStack();
	int stackFront();
	int* stackAt(int index);

private:
	//Additional Data 
	bitmap_image mImage;
	image_drawer mDrawer;

	//Colors
	std::vector<std::vector<int>> mColors
	{
		{255, 255, 255},
		{255, 255, 0},
		{255, 0, 255},
		{255, 0, 0},
		{0, 255, 255},
		{0, 255, 0},
		{0, 0, 255},
		{0, 0, 0},
	};

	//Ops
	std::vector<std::shared_ptr<Op>> mOps;
	std::map<std::string, std::function<std::shared_ptr<Op>()>> opMap;

	//Member Data
	std::map<std::string, int> mRegisters;
	std::map<std::string, bool> mFlags;
	std::vector<int> mStack;

	//Private Functions
	void initReg();
	std::ofstream mLog;
	std::ofstream mStackLog;
};
