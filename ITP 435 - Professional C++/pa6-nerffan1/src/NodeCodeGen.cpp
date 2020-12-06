#include "Node.h"
#include "parser.hpp"

void NBlock::CodeGen(CodeContext& context)
{
	for (auto state : mStatements)
	{
		state->CodeGen(context);
	}
}

void NData::CodeGen(CodeContext& context)
{
	for (auto decl : mDecls)
	{
		decl->CodeGen(context);
	}
}

void NProgram::CodeGen(CodeContext& context)
{
	mData->CodeGen(context);
	mMain->CodeGen(context);
	context.ops.emplace_back("exit");
}

void NNumeric::CodeGen(CodeContext& context)
{
	context.AddOp(
		"movi " +
		context.NextVirtlReg() + ',' +
		std::to_string(mValue));
}

void NVarDecl::CodeGen(CodeContext& context)
{
	context.ops.emplace_back("push r0");
	context.PushStack(mName);
}

void NArrayDecl::CodeGen(CodeContext& context)
{
	for (int i = 0; i < mSize->GetValue(); i++)
	{
		context.AddOp("push r0");
		if (i == 0)
		{
			context.PushStack(mName);
		}
		else
		{
			std::string name = mName + std::to_string(i);
			context.PushStack(name);
		}
	}
}

void NNumericExpr::CodeGen(CodeContext& context)
{
	mNumeric->CodeGen(context);
	mResultRegister = context.LastVirReg();
}

void NVarExpr::CodeGen(CodeContext& context)
{
	context.AddOp(
		"loadi " +
		context.NextVirtlReg() + ',' +
		context.GetStackIndex(mName)
	);
	mResultRegister = context.LastVirReg();
}

void NBinaryExpr::CodeGen(CodeContext& context)
{
	mLhs->CodeGen(context);
	mRhs->CodeGen(context);
	mResultRegister = context.NextVirtlReg();
	std::string op;
	switch (mType)
	{
	case TADD:
		op += "add ";
		break;
	case TSUB:
		op += "sub ";
		break;
	case TMUL:
		op += "mul ";
		break;
	case TDIV:
		op += "div ";
		break;
	default:
		break;
	}
	op += mResultRegister + ','
		+ mLhs->GetResultRegister() + ','
		+ mRhs->GetResultRegister();
	context.AddOp(op);
}

void NArrayExpr::CodeGen(CodeContext& context)
{
	mSubscript->CodeGen(context);
	//Movi base address 
	std::string index = context.NextVirtlReg();
	context.AddOp(
		"movi " +
		index + ',' +
		context.GetStackIndex(mName));

	//Add offset + base address
	std::string save = context.NextVirtlReg();
	context.AddOp(
		"add " +
		save + ',' +
		index + ',' +
		mSubscript->GetResultRegister()
	);
	context.AddOp(
		"load " +
		context.NextVirtlReg() + ',' +
		save
	);
	mResultRegister = context.LastVirReg();
}

void NAssignVarStmt::CodeGen(CodeContext& context)
{
	mRhs->CodeGen(context);
	context.AddOp(
		"storei " +
		context.GetStackIndex(mName) + ',' +
		mRhs->GetResultRegister());
}

void NAssignArrayStmt::CodeGen(CodeContext& context)
{
	mRhs->CodeGen(context);
	mSubscript->CodeGen(context);
	//Movi base address 
	std::string index = context.NextVirtlReg();
	context.AddOp(
		"movi " +
		index + ',' +
		context.GetStackIndex(mName));

	//Add offset + base address
	std::string last = context.LastVirReg();
	context.AddOp(
		"add " +
		context.NextVirtlReg() + ',' +
		index + ',' +
		mSubscript->GetResultRegister()
	);
	context.AddOp(
		"store " +
		context.LastVirReg() + ',' +
		mRhs->GetResultRegister()
	);
}

void NIncStmt::CodeGen(CodeContext& context)
{
	context.AddOp(
		"loadi " +
		context.NextVirtlReg() + ',' +
		context.GetStackIndex(mName)
	);
	context.AddOp("inc " + context.LastVirReg());
	context.AddOp(
		"storei " +
		context.GetStackIndex(mName) + ',' +
		context.LastVirReg()
	);
}

void NDecStmt::CodeGen(CodeContext& context)
{
	context.AddOp(
		"loadi " +
		context.NextVirtlReg() + ',' +
		context.GetStackIndex(mName)
	);
	context.AddOp("dec " + context.LastVirReg());
	context.AddOp(
		"storei " +
		context.GetStackIndex(mName) + ',' +
		context.LastVirReg()
	);
}

void NComparison::CodeGen(CodeContext& context)
{
	mLhs->CodeGen(context);
	mRhs->CodeGen(context);
	std::string cmp;
	switch (mType)
	{
	case TLESS:
		cmp += "cmplt ";
		break;
	case TISEQUAL:
		cmp += "cmpeq ";
		break;
	}
	cmp += mLhs->GetResultRegister() + ',';
	cmp += mRhs->GetResultRegister();
	context.AddOp(cmp);
}

void NIfStmt::CodeGen(CodeContext& context)
{
	mComp->CodeGen(context);
	context.AddOp(
		"movi " +
		context.NextVirtlReg() + ',');
	size_t i = context.ops.size() - 1; //Save
	context.AddOp(
		"jnt " +
		context.LastVirReg());
	mIfBlock->CodeGen(context); //Generate if block
	if (mElseBlock != nullptr)
	{
		context.AddOp(
			"movi " +
			context.NextVirtlReg() + ',');
		size_t j = context.ops.size() - 1;
		context.AddOp(
			"jmp " +
			context.LastVirReg());
		context.ops[i] += std::to_string(context.ops.size()); //Fix up address
		mElseBlock->CodeGen(context);
		context.ops[j] += std::to_string(context.ops.size()); //Fix up address
	}
	else
	{
		context.ops[i] += std::to_string(context.ops.size()); //Fix up address
	}
}

void NWhileStmt::CodeGen(CodeContext& context)
{
	size_t j = context.ops.size(); //Save
	mComp->CodeGen(context);
	context.AddOp(
		"movi " +
		context.NextVirtlReg() + ',');
	size_t k = context.ops.size() - 1; //Save
	context.AddOp(
		"jnt " +
		context.LastVirReg());
	mBlock->CodeGen(context);
	std::string tmp = context.NextVirtlReg();
	context.AddOp(
		"movi " +
		tmp + ',' +
		std::to_string(j));
	context.AddOp(
		"jmp " +
		context.LastVirReg());
	context.ops[k] += std::to_string(context.ops.size()); //Fix up address
}

void NPenUpStmt::CodeGen(CodeContext& context)
{
	context.AddOp("penup");
}

void NPenDownStmt::CodeGen(CodeContext& context)
{
	context.AddOp("pendown");
}

void NSetPosStmt::CodeGen(CodeContext& context)
{
	mXExpr->CodeGen(context);
	mYExpr->CodeGen(context);
	context.AddOp("mov tx," + mXExpr->GetResultRegister());
	context.AddOp("mov ty," + mYExpr->GetResultRegister());
}

void NSetColorStmt::CodeGen(CodeContext& context)
{
	mColor->CodeGen(context);
	context.AddOp("mov tc," + mColor->GetResultRegister());
}

void NFwdStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);
	context.AddOp("fwd " + mParam->GetResultRegister());
}

void NBackStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);
	context.AddOp("back " + mParam->GetResultRegister());
}

void NRotStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);
	context.AddOp("add tr,tr,"
		+ mParam->GetResultRegister());
}
