#pragma once
#include <string>
#include <fstream> 

class fasta
{
public:
	fasta(std::string fileName);

	//No default Constructor
	fasta() = delete;

protected:
	std::string mHead;
	std::string mSequence;
	friend class DNATrans;
	friend class alignment;
};