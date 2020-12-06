#include "RleData.h"
#include <sstream>
#include <string>
#include <cmath>

void RleData::Compress(const char* input, size_t inSize)
{
	// TODO
	delete[] mData;
	mSize = 0;

	// Allocate space for mData and write first prefix 
	mData = new char[2 * inSize];
	Inputptr = input;

	//Compress
	bool* duplicates = new bool[inSize - 1]; //This array will help me with writing the compressed version
	for (size_t i = 0; i < (inSize - 1); i++)
	{
		duplicates[i] = (input[i] == input[i + 1]); //If the index and next index are equal, then
	}												//the value is true. This will be defined as 

	//Loop through 
	size_t beg{ 0 };
	for (size_t i = 1; i < (inSize - 1); i++)
	{
		bool sign{ duplicates[i - 1] };
		if (sign) //Positive Run
		{
			if (i == inSize - 2) //Edge case
			{
				if (duplicates[i])
				{
					CompressWrite(beg, i + 1, sign);
				}
				else
				{
					CompressWrite(beg, i, sign);
					beg = i + 1;
					CompressWrite(beg, beg, false);
				}
			}
			else
			{
				if (!duplicates[i]) //If the next one is negative, then write up to this point.
				{
					CompressWrite(beg, i, sign);
					beg = i + 1;
				}
			}
		}
		else //Negative Run
		{
			if (i == inSize - 2) //Edge Case
			{
				if (duplicates[i])
				{
					CompressWrite(beg, i - 1, sign);
					beg = i;
					CompressWrite(beg, i + 1, !sign);
				}
				else
				{
					CompressWrite(beg, i + 1, sign);
				}
			}
			else
			{
				if (duplicates[i] && !duplicates[i - 2])
				{
					if (beg == i - 1)
					{
						CompressWrite(beg, i - 1, true);
					}
					else
					{
						CompressWrite(beg, i - 1, sign);
					}
					beg = i;
				}
			}
		}
	}
	delete[] duplicates;
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	// TODO
	mSize = 0;

	// Allocate space for mData and write first prefix 
	mData = new char[outSize];
	mSize = static_cast<int>(outSize);
	Inputptr = input;
	size_t readBeg{ 0 };
	size_t writeBeg{ 0 };
	DecompressWrite(inSize, readBeg, writeBeg);
}

void RleData::CompressWrite(size_t beg, const size_t& end, const bool& sign)
{
	//Determine hex number to write
	int length{ static_cast<int>(end - beg + 1) };
	size_t run{ static_cast<size_t>(length / 127) };
	int remainder{ length % 127 };
	size_t index{ beg };

	if (run > 0)
	{
		for (size_t i = 0; i < run; i++)
		{
			char hex = sign ? 127 : -127; //Determine negative or sign run

			InsertData(hex);

			if (sign)
			{
				InsertData(Inputptr[index++]);
			}
			else
			{
				//Insert the sign/negative data
				for (size_t j = 0; j < 127; j++)
				{
					InsertData(Inputptr[index++]);
				}
			}
		}
	}

	//Insert the sign/negative data
	if (remainder != 0)
	{
		//Insert number of run
		char hex;
		if (remainder == 1)
		{
			hex = 1;
		}
		else
		{
			hex = sign ? remainder : -remainder; //Determine negative or sign run
		}
		InsertData(hex);
		//Insert the data
		if (sign)
		{
			InsertData(Inputptr[index]);
		}
		else
		{
			for (size_t j = 0; j < static_cast<size_t>(remainder); j++)
			{
				InsertData(Inputptr[index++]);
			}
		}
	}
}

void RleData::InsertData(char in)
{
	mData[LastElement++] = in;
	mSize++;
}

void RleData::DecompressWrite(size_t& inSize, size_t& Readbeg, size_t& WriteBeg)
{
	int runNum{ static_cast<int>(Inputptr[Readbeg]) };
	size_t count{ static_cast<size_t>(abs(runNum)) };
	for (size_t i = 0; i < count; i++)
	{
		mData[WriteBeg++] = (runNum > 0) ? Inputptr[Readbeg + 1] : Inputptr[++Readbeg];
	}
	Readbeg = (runNum > 0) ? Readbeg + 2 : ++Readbeg;
	if (Readbeg < inSize)
	{
		DecompressWrite(inSize, Readbeg, WriteBeg);
	}
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}
