#include "SrcMain.h"
#include <iostream>
#include "Machine.h"
#include "Exceptions.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO
	Machine itpMachine;
	itpMachine.ReadAseembly(argv[1]);
	
	while (!itpMachine.GetFlag("exit") )
	{
		try
		{
			itpMachine.Execute();
		}
		catch (Fatal& e)
		{
			itpMachine.SetRegister("ex", e.mCode);
			itpMachine.SetFlag("exit", true);
		}
		catch (Nonfatal& e)
		{
			itpMachine.SetRegister("ex", e.mCode);
		}
	}
}
