#include "SrcMain.h"
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <iostream>
#include "Node.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <tuple>
#include <algorithm>

using replaceInfo = std::pair<size_t, size_t>;
using regAndLine = std::pair<std::string, int>;
using replacements = std::vector<std::pair<replaceInfo, regAndLine>>;
using cleanLines = std::vector<std::vector<std::string>>;
using virtualMap = std::map<std::string, std::string>;
using regranges = std::vector<std::pair<int, int>>;
using registerAllocation = std::map<std::string, bool>;

//My Functions
void ReplaceReg(std::ofstream& of);
void VirtRegNum(std::vector<std::string>& ops, std::vector<std::string>& virtualRegs, regranges& startEnd, replacements& reps);
void Print(std::ofstream& of, cleanLines& con, virtualMap& virtToReal, regranges& startEnds);
cleanLines SeparateOps(CodeContext& con);
void Scan(regranges& startEnds, virtualMap& virtToReal, std::vector<std::string>& virtualRegs, registerAllocation realRegs);

extern int proccparse(); // NOLINT
struct yy_buffer_state; // NOLINT
extern void procc_flush_buffer(yy_buffer_state* b); // NOLINT
extern int procclex_destroy(); // NOLINT
extern FILE* proccin; // NOLINT

extern int gLineNumber;
extern NProgram* gProgram;
bool gSyntaxError = false;

// CHANGE ANYTHING ABOVE THIS LINE AT YOUR OWN RISK!!!!

int ProcessCommandArgs(int argc, const char* argv[])
{
	gLineNumber = 1;
	gSyntaxError = false;
	if (argc < 2)
	{
		std::cout << "You must pass the input file as a command line parameter." << std::endl;
		return 1;
	}

	// Read input from the first command line parameter
	proccin = fopen(argv[1], "r");
	if (proccin == nullptr)
	{
		std::cout << "File not found: " << argv[1] << std::endl;
		return 1;
	}
	procc_flush_buffer(nullptr);

	// Start the parse
	proccparse();

	if (gProgram != nullptr && argc == 3)
	{
		// TODO: Add any needed code for Parts 2, 3, and 4!!!
		std::ofstream mOutput("ast.txt");
		gProgram->OutputAST(mOutput, 0);

		std::string tm = argv[2];

		if (tm == "emit")
		{
			CodeContext myContxt;
			gProgram->CodeGen(myContxt);
			std::ofstream mOutput("emit.txt");

			for (auto i : myContxt.ops)
			{
				mOutput << i << '\n';
			}
		}
		if (tm == "reg")
		{
			std::ofstream mOutput("emit.txt");
			ReplaceReg(mOutput);
		}
	}
	else
	{
		// (Just a useful separator for debug cout statements in grammar)
		std::cout << "**********************************************\n";
	}
	// Close the file stream
	fclose(proccin);
	// Destroy lexer so it reinitializes
	procclex_destroy();
	// Return 1 if syntax error, 0 otherwise
	return static_cast<int>(gSyntaxError);
}

void proccerror(const char* s) // NOLINT
{
	std::cout << s << " on line " << gLineNumber << std::endl;
	gSyntaxError = true;
}

void ReplaceReg(std::ofstream& of)
{
	//Emit instructions
	CodeContext myContxt;
	gProgram->CodeGen(myContxt);

	//Variables 
	registerAllocation realRegs
	{ {"r1",true},{"r2",true}, {"r3",true}, {"r4",true},
	{"r5",true}, {"r6",true}, {"r7",true} };
	virtualMap virtToReal;
	replacements reps;

	/*__________________________LINEAR SCAN_____________________________*/

	//PT1 : Generate Intervals for individual virtual registers
	std::vector<std::string> virtualRegs;
	regranges startEnds;
	VirtRegNum(myContxt.ops, virtualRegs, startEnds, reps);
	cleanLines mySepLines = SeparateOps(myContxt);

	//Create map for virtual-to-real registers
	for (auto vtr : virtualRegs)
	{
		virtToReal[vtr];
	}

	//PT2 : Scan by looping through the startEnds and check for expired, or new registers from r1-r7
	Scan(startEnds, virtToReal, virtualRegs, realRegs);

	//Replace virtual registers with real ones
	for (auto& line : mySepLines)
	{
		for (auto& opreg : line)
		{
			if (virtToReal.find(opreg) != virtToReal.end())
			{
				opreg = virtToReal[opreg];
			}
		}
	}

	//PrintInfo
	Print(of, mySepLines, virtToReal, startEnds);
}

void VirtRegNum(std::vector<std::string>& ops, std::vector<std::string>& virtualRegs, regranges& startEnd, replacements& reps)
{
	std::string tmp;
	size_t lastr;
	size_t lastspace;
	for (auto& l : ops)
	{
		short inOneLine = 0;
		lastr = 0;
		while (lastr != std::string::npos)
		{
			lastr = l.find_first_of('%', lastr + 1);
			if (lastr != std::string::npos) //If there is a Virtual Register here!
			{
				//Get virtual register into tmp

				lastspace = l.find_first_of(',', lastr);

				if (lastspace == std::string::npos)
				{
					lastspace = l.size();
				}

				size_t regsiz = lastspace - lastr;
				tmp = l.substr(lastr, regsiz);

				//Get the linenumber of tmp
				auto end = std::find(ops.begin(), ops.end(), l);
				int linenum = std::distance(ops.begin(), end);

				//Check if the register exists already, or not. Save it's line number.
				auto isitthere = std::find(virtualRegs.begin(), virtualRegs.end(), tmp);
				if (isitthere == virtualRegs.end()) //If the register hasn't been encountered before
				{
					virtualRegs.emplace_back(tmp);
					startEnd.emplace_back(std::pair<int, int>(linenum, -1));
				}
				else //If we've found this before, then
				{
					size_t index = std::distance(virtualRegs.begin(), isitthere);
					startEnd[index].second = linenum;
				}
				reps.emplace_back(replaceInfo(lastr, regsiz), regAndLine(tmp, linenum));
				++inOneLine;
			}
		}
	}
}

void Print(std::ofstream& of, cleanLines& con, virtualMap& virtToReal, regranges& startEnds)
{
	//Print emit 
	int k = 0;
	for (auto line : con)
	{
		for (auto opreg : line)
		{
			if (k == 0)
			{
				of << opreg;
			}
			else if (k == 1)
			{
				of << " ";
				of << opreg;
			}
			else
			{
				of << ',';
				of << opreg;
			}
			++k;
		}
		k = 0;
		of << '\n';
	}
	of.close();

	//Print Reg
	std::ofstream printReg("reg.txt");
	printReg << "INTERVALS:\n";
	for (auto i = 0; i < startEnds.size(); i++)
	{
		auto range = startEnds[i];
		printReg << '%' << i << ':';
		printReg << range.first << ',' << range.second;
		printReg << '\n';
	}
	printReg << "ALLOCATION:\n";
	for (auto i = 0; i < virtToReal.size(); i++)
	{
		auto virt = "%" + std::to_string(i);
		auto real = virtToReal[virt];
		printReg << virt << ':' << real;
		printReg << '\n';
	}
}

cleanLines SeparateOps(CodeContext& con)
{
	cleanLines newCleanLines(con.ops.size());
	enum statetype { OP, REGS };
	int type = OP;
	size_t line = 0;
	size_t start = 0;
	size_t end = 0;
	std::string tmp;
	for (auto l : con.ops)
	{
		while (end != l.size())
		{
			switch (type)
			{
			case OP:
				end = l.find_first_of(' ', start);
				type = REGS;
				break;
			case REGS:
				++start;
				end = l.find_first_of(',', start);
				if (end == std::string::npos)
				{
					end = l.size();
				}
				break;
			}
			tmp = l.substr(start, end - start);
			newCleanLines[line].emplace_back(tmp);
			if (tmp == "exit" || tmp == "pendown" || tmp == "pendown"
				|| tmp == "penup") {break;}
			start = end;
		}
		//Reset values
		++line;
		end = 0;
		start = 0;
		type = OP;
	}
	return newCleanLines;
}

void Scan(regranges& startEnds, virtualMap& virtToReal, std::vector<std::string>& virtualRegs, registerAllocation realRegs)
{
	//Helper variable to keep track of activated virtual registers
	std::vector<bool> dontcheck(startEnds.size());

	//Loop through the START-END vector and determine the mapping 
	for (auto line = 0; line < startEnds.size(); line++)
	{
		//Free real-registers
		for (auto checkToFree = 0; checkToFree < line; checkToFree++)
		{
			if (!dontcheck[checkToFree]) //Make sure this hasn't been freed before
			{
				if (startEnds[checkToFree].second <= startEnds[line].first)
				{
					auto realToFree = virtToReal[virtualRegs[checkToFree]];
					realRegs[realToFree] = true; //free it !
					dontcheck[checkToFree] = true; //change so that it doesn't get checked again
				}
			}
		}

		//Assign real-register to virtual ones
		for (auto& rreg : realRegs)
		{
			if (rreg.second)
			{
				virtToReal[virtualRegs[line]] = rreg.first; //Assign
				rreg.second = false; //Make unavailable
				break;
			}
		}
	}
}
