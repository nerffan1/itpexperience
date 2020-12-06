#include "Ops.h"
#include "Machine.h"

void Exit::Execute(Machine& machine)
{
	// TODO
	machine.SetFlag("exit", true);
}

//Behavior: Sets reg1 = int
void MovI::Execute(Machine& machine)
{
	// TODO
	machine.SetRegister(std::get<0>(mParameters), std::get<1>(mParameters));
}

//Behavior: reg1 = reg2 + reg3
void Add::Execute(Machine& machine)
{
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	int reg3 = machine.GetRegister(std::get<2>(mParameters));
	machine.SetRegister(std::get<0>(mParameters), reg2 + reg3);
	CheckFlow(reg2, reg3, 'a');
}

//Behavior: reg1 = reg2
void Mov::Execute(Machine& machine)
{
	machine.SetRegister(
		std::get<0>(mParameters),
		machine.GetRegister(std::get<1>(mParameters)));
}

//Behavior: reg1 = reg2 - reg3
void Sub::Execute(Machine& machine)
{
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	int reg3 = machine.GetRegister(std::get<2>(mParameters));
	machine.SetRegister(std::get<0>(mParameters), reg2 - reg3);
	CheckFlow(reg2, reg3, 's');
}

//Behavior: reg1 = reg2 * reg3
void Mul::Execute(Machine& machine)
{
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	int reg3 = machine.GetRegister(std::get<2>(mParameters));
	machine.SetRegister(std::get<0>(mParameters), reg2 * reg3);
	CheckFlow(reg2, reg3, 'm');

}

//Behavior: reg1 = reg2 / reg3
void Div::Execute(Machine& machine)
{
	int reg3 = machine.GetRegister(std::get<2>(mParameters));
	if (reg3 == 0)
	{
		throw Fatal(102);
	}
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	machine.SetRegister(std::get<0>(mParameters), reg2 / reg3);
	CheckFlow(reg2, reg3, 'd');
}

//Behavior: reg1++

void Inc::Execute(Machine& machine)
{
	int reg1 = machine.GetRegister(std::get<0>(mParameters));
	++reg1;
	machine.SetRegister(std::get<0>(mParameters), reg1);
	CheckFlow(reg1, 0, 'i');
}

void Dec::Execute(Machine& machine)
{
	int reg1 = machine.GetRegister(std::get<0>(mParameters));
	machine.SetRegister(std::get<0>(mParameters), --reg1);
}

//Behavior: pc = reg1
void Jmp::Execute(Machine& machine)
{
	machine.SetRegister("pc",
		machine.GetRegister(std::get<0>(mParameters)));
}

//Behavior: test = (reg1 == reg2)
void Cmpeq::Execute(Machine& machine)
{
	int reg1 = machine.GetRegister(std::get<0>(mParameters));
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	bool testV = (reg1 == reg2);
	machine.SetFlag("test", testV);
}

//Behavior: test = (reg1 < reg2) 
void Cmplt::Execute(Machine& machine)
{
	int reg1 = machine.GetRegister(std::get<0>(mParameters));
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	bool testV = (reg1 < reg2);
	machine.SetFlag("test", testV);
}

//Behavior: If test == true, then set pc = reg1
void Jt::Execute(Machine& machine)
{
	if (machine.GetFlag("test"))
	{
		int reg1 = machine.GetRegister(std::get<0>(mParameters));
		machine.SetRegister("pc", reg1);
	}
}

//Behavior: If test == false, then set pc = reg1
void Jnt::Execute(Machine& machine)
{
	if (!machine.GetFlag("test"))
	{
		int reg1 = machine.GetRegister(std::get<0>(mParameters));
		machine.SetRegister("pc", reg1);
	}
}

//Behavior: Pushes reg1 onto stack, and increments the sc register
void Push::Execute(Machine& machine)
{
	int reg1 = machine.GetRegister(std::get<0>(mParameters));
	machine.pushStack(reg1);
	int newSc = machine.GetRegister("sc") + 1;
	machine.popPushReg("sc", newSc);
}

//Behavior: Pops top of stack into reg1, and decrements the sc register
void Pop::Execute(Machine& machine)
{
	machine.SetRegister(std::get<0>(mParameters),
		machine.stackFront());
	machine.popStack();
	int newSc = machine.GetRegister("sc") - 1;
	machine.popPushReg("sc", newSc);
}

//Behavior: reg1 = stack[reg2]
void Load::Execute(Machine& machine)
{
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	machine.SetRegister(
		std::get<0>(mParameters),
		*machine.stackAt(reg2)
	);
}

//Behavior: stack[reg1] = reg2
void Store::Execute(Machine& machine)
{
	int reg1 = machine.GetRegister(std::get<0>(mParameters));
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	*machine.stackAt(reg1) = reg2;
}

//Behavior: reg1 = stack[int]
void LoadI::Execute(Machine& machine)
{
	int index = std::get<1>(mParameters);
	machine.SetRegister(std::get<0>(mParameters),
		*machine.stackAt(index));
}

//Behavior: stack[int] = reg1
void StoreI::Execute(Machine& machine)
{
	int reg1 = machine.GetRegister(std::get<1>(mParameters));
	*machine.stackAt(std::get<0>(mParameters)) = reg1;
}

//Behaviour: reg1 = stack[sc – reg2 – 1]
void LoadSc::Execute(Machine& machine)
{
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	int sc = machine.GetRegister("sc");
	machine.SetRegister(std::get<0>(mParameters),
		*machine.stackAt(sc - reg2 - 1));
}

//Behaviour: stack[sc – reg1 – 1] = reg2
void StoreSc::Execute(Machine& machine)
{
	int sc = machine.GetRegister("sc");
	int reg1 = machine.GetRegister(std::get<0>(mParameters));
	int reg2 = machine.GetRegister(std::get<1>(mParameters));
	*machine.stackAt(sc - reg1 - 1) = reg2;
}

void PenDown::Execute(Machine& machine)
{
	machine.SetFlag("pen", true);
}

void PenUp::Execute(Machine& machine)
{
	machine.SetFlag("pen", false);
}

void Fwd::Execute(Machine& machine)
{
	machine.Turtle(std::get<0>(mParameters));
}

void Back::Execute(Machine& machine)
{
	machine.Turtle(std::get<0>(mParameters), false);
}
