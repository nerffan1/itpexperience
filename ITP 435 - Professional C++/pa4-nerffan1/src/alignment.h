#pragma once
#include <vector>
#include <string>


class fasta;

enum direction : char
{
	aboveLeft,
	left,
	above
};

class alignment
{
public:
	alignment(fasta &pFile1, fasta &pFile2);
	alignment() = delete;
	void Process();
	short GetScore(size_t& i, size_t& j);
	void Align(size_t& size1, size_t& size2);
	void InitiateRowColumn(size_t &size1, size_t &size2);
	void FindMatch(size_t& size1, size_t& size2);
	void Print(size_t& size1, size_t& size2);


private:
	//Fasta members
	fasta* fasta1;
	fasta* fasta2;

	//Grids
	std::vector<std::vector<char>> mDirGrid;
	std::vector<std::vector<short>> mScoreGrid;

	//Matched Strings
	std::string match1;
	std::string match2;
	size_t maxMatchSize;
	//Consts
	const short MATCH_SCORE = 1;
	const short MISMATCH_SCORE = -1;
	const short GAP_SCORE = -1;

	//Helper Variables for Scoring
	short value1, value2, value3, tmp;
	
};
