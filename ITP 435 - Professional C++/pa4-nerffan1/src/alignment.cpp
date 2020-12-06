#include "alignment.h"
#include "fasta.h"
#include <fstream>

alignment::alignment(fasta& pFile1, fasta& pFile2)
{

	//Initialize member variables
	fasta1 = &pFile1;
	fasta2 = &pFile2;

	//Populate Grids
	Process();
}

void alignment::Process()
{
	//Give the strings an empty initial space
	fasta1->mSequence.insert(0, "_");
	fasta2->mSequence.insert(0, "_");

	//Allocate Memory
	size_t size1 = fasta1->mSequence.size();
	mDirGrid.resize(size1);
	mScoreGrid.resize(size1);

	size_t size2 = fasta2->mSequence.size();
	for (auto i = 0; i < size1; i++)
	{
		mDirGrid[i].resize(size2);
		mScoreGrid[i].resize(size2);
	}

	//Define Max 
	maxMatchSize = (size1 >= size2) ? size1 : size2;

	//Fill the score grid
	InitiateRowColumn(size1, size2);
	Align(size1, size2);

	//Get Matches
	FindMatch(size1, size2);

	//Print Matches 
	Print(size1, size2);
}

short alignment::GetScore(size_t& i, size_t& j)
{
	bool match = fasta1->mSequence[i] == fasta2->mSequence[j];
	if (match)
	{
		value1 = mScoreGrid[i - 1][j - 1] + MATCH_SCORE;
	}
	else
	{
		value1 = mScoreGrid[i - 1][j - 1] + MISMATCH_SCORE;
	}

	//Value2 
	value2 = mScoreGrid[i][j - 1] + GAP_SCORE;

	//Value3
	value3 = mScoreGrid[i - 1][j] + GAP_SCORE;

	//Get Max and assign direction in Direction Grid
	if (i == 0 && j != 0)
	{
		mDirGrid[i][j] = left;
	}
	else if (j == 0 && i != 0)
	{
		mDirGrid[i][j] = above;
	}
	else {
		tmp = std::max(value1, value2);
		tmp = std::max(tmp, value3);

		if (value2 == tmp)
		{
			mDirGrid[i][j] = left;
		}
		if (value3 == tmp)
		{
			mDirGrid[i][j] = above;
		}
		if (value1 == tmp)
		{
			mDirGrid[i][j] = aboveLeft;
		}
	}
	return tmp;
}

void alignment::Align(size_t& size1, size_t& size2)
{
	for (size_t i = 1; i < size1; ++i)
	{
		for (size_t j = 1; j < size2; ++j)
		{
			mScoreGrid[i][j] = GetScore(i, j);
		}
	}
}

void alignment::InitiateRowColumn(size_t& size1, size_t& size2)
{
	//Initiate scores of first column
	for (auto i = 0; i < maxMatchSize; i++)
	{
		if (i < size1)
		{
			mScoreGrid[i][0] = -static_cast<short>(i);
		}

		//Also declare value for rows as to not repeat another full loop
		if (i < size2)
		{
			mScoreGrid[0][i] = -static_cast<short>(i);
		}

	}

	//Allocate space in strings
	match1.resize(size1 + size2);
	match2.resize(size1 + size2);
}

void alignment::FindMatch(size_t& size1, size_t& size2)
{
	size_t sizeOfMatch = 0;
	size_t k = match1.size() - 1;
	size_t i = size1 - 1;
	size_t j = size2 - 1;
	while (i != 0 || j != 0)
	{
		switch (mDirGrid[i][j])
		{
		case aboveLeft:
		{
			match1[k] = fasta1->mSequence[i];
			match2[k] = fasta2->mSequence[j];
			if (i != 0) { --i; }
			if (j != 0) { --j; }
			break;
		}
		case left:
		{
			match1[k] = '_';
			match2[k] = fasta2->mSequence[j];
			if (j != 0) { --j; }
			break;
		}
		case above:
		{
			match1[k] = fasta1->mSequence[i];
			match2[k] = '_';
			if (i != 0) { --i; }
			break;
		}
		}
		++sizeOfMatch;
		--k;
	}
	match1.erase(0, match1.size() - sizeOfMatch);
	match2.erase(0, match2.size() - sizeOfMatch);
}

void alignment::Print(size_t& size1, size_t& size2)
{
	std::ofstream output("match.result");
	if (output.is_open())
	{
		//Write Headers 
		output << "A: " << fasta1->mHead << "\n";
		output << "B: " << fasta2->mHead << "\n";
		output << "Score: " << mScoreGrid[size1 - 1][size2 - 1];
		output.write("\n\n", 2);

		//Write to file
		size_t matchSticks = 0;
		size_t match2Index = 0;
		size_t last = 0;
		size_t matchSize = match1.size();
		for (size_t i = 0; i < matchSize; i++)
		{
			output.write(&match1[i], 1);

			//For every 70 characters: new line
			if ((i + 1) % 70 == 0)
			{
				last = i + 1;
				output << "\n";
				for (auto k = 0; k < 70; k++)
				{
					if (match1[matchSticks] == match2[matchSticks])
					{
						output.write("|", 1);
					}
					else
					{
						output.write(" ", 1);
					}
					++matchSticks;
				}
				output.write("\n", 1);
				for (auto k = 0; k < 70; k++)
				{
					output.write(&match2[match2Index], 1);
					++match2Index;
				}
				output.write("\n", 1);
				output.write("\n", 1);

			}
		}
		//Write the rest
		output.write("\n", 1);
		size_t anotherindex = match2Index;
		while (last < matchSize)
		{

			if (match1[last] == match2[anotherindex])
			{
				output.write("|", 1);
			}
			else
			{
				output.write(" ", 1);
			}
			++last;
			++anotherindex;
		}
		output.write("\n", 1);
		while (match2Index < matchSize)
		{
			output.write(&match2[match2Index], 1);
			++match2Index;
		}


		output.close();
	}
}
