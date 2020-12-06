#include "Machine.h"
#include "Exceptions.h"

Machine::Machine() : mImage(320, 240), mDrawer(mImage)
{
	//Emplace all operators
	opMap.emplace("movi", &CreateOp<MovI>);
	opMap.emplace("exit", &CreateOp<Exit>);
	opMap.emplace("add", &CreateOp<Add>);
	opMap.emplace("mov", &CreateOp<Mov>);
	opMap.emplace("sub", &CreateOp<Sub>);
	opMap.emplace("mul", &CreateOp<Mul>);
	opMap.emplace("div", &CreateOp<Div>);
	opMap.emplace("inc", &CreateOp<Inc>);
	opMap.emplace("dec", &CreateOp<Dec>);
	opMap.emplace("jmp", &CreateOp<Jmp>);
	opMap.emplace("cmpeq", &CreateOp<Cmpeq>);
	opMap.emplace("cmplt", &CreateOp<Cmplt>);
	opMap.emplace("jt", &CreateOp<Jt>);
	opMap.emplace("jnt", &CreateOp<Jnt>);
	//Stack Based
	opMap.emplace("push", &CreateOp<Push>);
	opMap.emplace("pop", &CreateOp<Pop>);
	opMap.emplace("load", &CreateOp<Load>);
	opMap.emplace("store", &CreateOp<Store>);
	opMap.emplace("loadi", &CreateOp<LoadI>);
	opMap.emplace("storei", &CreateOp<StoreI>);
	opMap.emplace("loadsc", &CreateOp<LoadSc>);
	opMap.emplace("storesc", &CreateOp<StoreSc>);
	//Bitmap Ops
	opMap.emplace("pendown", &CreateOp<PenDown>);
	opMap.emplace("penup", &CreateOp<PenUp>);
	opMap.emplace("fwd", &CreateOp<Fwd>);
	opMap.emplace("back", &CreateOp<Back>);

	//Initiate Registers 
	initReg();
	mLog.open("log.txt", std::ios::trunc);
	mStackLog.open("stack.txt", std::ios::trunc);
	PrintStack();

	//Initiate image
	mImage.set_all_channels(0, 0, 0);
	mDrawer.pen_width(1);
	mDrawer.pen_color(255, 255, 255);
}

void Machine::ReadAseembly(std::string pFile)
{
	std::ifstream readPsm(pFile);
	if (readPsm.is_open())
	{
		std::string line;
		std::string opName;
		std::string params;
		while (!readPsm.eof())
		{
			std::getline(readPsm, line);
			auto space = line.find(' ');
			if (space != std::string::npos)
			{
				opName = line.substr(0, space);
				++space;
				params = line.substr(space, line.size() - space);
			}
			else
			{
				opName = line;
			}
			mOps.emplace_back(opMap.at(opName)());

			//Parse the paramters in proper format
			if (!params.empty())
			{
				mOps.back()->Parse(params);
			}
			opName.clear();
			params.clear();
		}
	}
}

void Machine::Execute()
{
	int pcReg;
	std::shared_ptr<Op> pcOp;

	while (!mFlags["exit"])
	{
		pcReg = mRegisters["pc"];
		pcOp = mOps[pcReg];

		//Log Update
		PrintInfo();

		//Print Executed Op
		mLog << "Executing: ";
		mLog << pcOp->GetName() << "\n";

		//Execute
		++mRegisters["pc"];
		pcOp->Execute(*this);
		PrintStack();
	}

	// Image save
	mImage.save_image("output.bmp");
}

void Machine::Turtle(std::string reg1, bool pos)
{
	int regint = mRegisters[reg1];
	if (!pos) { regint *= -1; }

	int tx = mRegisters["tx"];
	int ty = mRegisters["ty"];
	float ctrr = mRegisters["tr"] * 0.0174533f;
	int endx = tx +
		static_cast<int>(std::cos(ctrr) * regint);
	int endy = ty +
		static_cast<int>(std::sin(ctrr) * regint);

	if (mFlags["pen"])
	{
		std::vector<int> color = mColors[mRegisters["tc"]];
		mDrawer.pen_color(color[0], color[1], color[2]);
		mDrawer.line_segment(tx, ty, endx, endy);
	}

	//Update tx ty
	mRegisters["tx"] = endx;
	mRegisters["ty"] = endy;
}

void Machine::PrintInfo()
{
	//Print to Log
	mLog << "********************\n";
	/*for (auto reg : mRegisters)
	{
		mLog << reg.first << "=";
		mLog << reg.second << "\n";
	}
	for (auto flag : mFlags)
	{
		mLog << flag.first << "=";
		mLog << flag.second << "\n";
	}*/

	mLog << "r0=" << mRegisters["r0"] << "\n";
	mLog << "r1=" << mRegisters["r1"] << "\n";
	mLog << "r2=" << mRegisters["r2"] << "\n";
	mLog << "r3=" << mRegisters["r3"] << "\n";
	mLog << "r4=" << mRegisters["r4"] << "\n";
	mLog << "r5=" << mRegisters["r5"] << "\n";
	mLog << "r6=" << mRegisters["r6"] << "\n";
	mLog << "r7=" << mRegisters["r7"] << "\n";
	mLog << "pc=" << mRegisters["pc"] << "\n";
	mLog << "sc=" << mRegisters["sc"] << "\n";
	mLog << "ex=" << mRegisters["ex"] << "\n";
	mLog << "tx=" << mRegisters["tx"] << "\n";
	mLog << "ty=" << mRegisters["ty"] << "\n";
	mLog << "tr=" << mRegisters["tr"] << "\n";
	mLog << "tc=" << mRegisters["tc"] << "\n";
	mLog << "exit=" << mFlags["exit"] << "\n";
	mLog << "test=" << mFlags["test"] << "\n";
	mLog << "pen=" << mFlags["pen"] << "\n";
}

void Machine::PrintStack()
{
	//Print Stack info
	mStackLog << "pc: " << mRegisters["pc"];
	mStackLog << " Stack: ";
	for (auto s : mStack)
	{
		mStackLog << s << " ";
	}
	mStackLog << "\n";
}

void Machine::SetRegister(std::string p, int value)
{
	if (p == "sc")
	{
		throw Nonfatal(10);
	}
	if (p == "r0")
	{
		throw Nonfatal(11);
	}
	if (p == "tc")
	{
		if (value < 0 || value > 7)
		{
			throw Nonfatal(14);
		}
	}
	mRegisters[p] = value;
}

void Machine::popPushReg(std::string p, int value) noexcept
{
	mRegisters[p] = value;
}

void Machine::pushStack(int& pIn)
{
	if (mStack.size() == 256)
	{
		throw Fatal(101);
	}

	{ mStack.push_back(pIn); }
}

void Machine::popStack()
{
	if (mStack.empty())
	{
		throw Fatal(103);
	}
	mStack.pop_back();
}

int Machine::stackFront()
{
	if (mStack.empty())
	{
		throw Fatal(103);
	}
	return mStack.back();
}

int* Machine::stackAt(int index)
{
	if (index < 0 || index >= mRegisters["sc"])
	{
		throw Fatal(100);
	}
	return &mStack[static_cast<size_t>(index)];
}

void Machine::initReg()
{
	//Initiate Registers
	mRegisters["r0"];
	mRegisters["r1"];
	mRegisters["r2"];
	mRegisters["r3"];
	mRegisters["r4"];
	mRegisters["r5"];
	mRegisters["r6"];
	mRegisters["r7"];
	mRegisters["pc"];
	mRegisters["sc"];
	mRegisters["ex"];
	mRegisters["tx"];
	mRegisters["ty"];
	mRegisters["tr"];
	mRegisters["tc"];

	//Initiate Flags
	mFlags["test"];
	mFlags["exit"];
	mFlags["pen"];
}


