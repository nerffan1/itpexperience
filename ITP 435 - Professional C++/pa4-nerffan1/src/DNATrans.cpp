#include "DNATrans.h"
#include "fasta.h"
#include <fstream>

void DNATrans::PrintResults()
{
	std::ofstream mResults("amino.txt");
	if (mResults.is_open())
	{
		mResults << mFasta->mHead << "\n";
		mResults << "Total amino acids produced: " << mTotalAA << "\n";
		char base;
		for (auto amino : mAminoName)
		{
			base = amino.first;
			mResults << "(" << base << ") ";
			mResults << amino.second << ": ";
			mResults << mAminoCount[base] << "\n";
		}
		mResults.close();
	}
}

void DNATrans::AnalyzeCodons()
{
	size_t stringSize = mFasta->mSequence.size();
	char base;
	char NewState;
	//Loop through all bases
	for (size_t i = 0; i < stringSize; i++)
	{
		//Get Base
		base = mFasta->mSequence[i];

		//Find State
		NewState = mStateMachine[mCurrState][mBaseNum[base]];

		if (NewState >= 'A' && NewState < 'Z') //Reaching a codon
		{
			++mAminoCount[NewState];
			++mTotalAA;
			mCurrState = 3;
		}
		else if (NewState >= 0 && NewState <= 23)
		{
			mCurrState = NewState;
		}
		else if (NewState == 'x') //Stop Codon
		{
			mCurrState = 0;
		}

	}
	PrintResults();
}