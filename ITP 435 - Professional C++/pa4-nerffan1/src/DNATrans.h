#pragma once
#include <map>
#include <string>

class fasta;

class DNATrans
{
public:
	//Constructors
	DNATrans(fasta& pSequence) : mFasta(&pSequence) { AnalyzeCodons(); }
	DNATrans() = delete;

	//Functions
	void PrintResults();
	void AnalyzeCodons();

private:
	//State
	fasta* mFasta;
	int mTotalAA = 0;
	char mCurrState = 0;

	//Map to keep track of amino acids
	std::map<char, int> mAminoCount{
		{'A',0 }, {'C',0 }, {'D',0 }, {'E',0 },
		{'F',0 }, {'G',0 }, {'H',0 }, {'I',0 },
		{'K',0 }, {'L',0 }, {'M',0 }, {'N',0 },
		{'P',0 }, {'Q',0 }, {'R',0 }, {'S',0 },
		{'T',0 }, {'V',0 }, {'W',0 }, {'Y',0 }
	};

	//Map to keep track of names
	std::map<char, std::string> mAminoName{
		{'A',"Alanine" }, {'C',"Cysteine" }, {'D',"Aspartic acid" }, {'E',"Glutamic acid" },
		{'F',"Phenylalanine" }, {'G',"Glycine" }, {'H',"Histidine"}, {'I',"Isoleucine" },
		{'K',"Lysine" }, {'L',"Leucine" }, {'M',"Methionine" }, {'N',"Asparagine"},
		{'P',"Proline" }, {'Q',"Glutamine" }, {'R',"Arginine"}, {'S',"Serine"},
		{'T',"Threonine" }, {'V',"Valine" }, {'W',"Tryptophan" }, {'Y',"Tyrosine" }
	};

	//Map to access stateMachine
	std::map<char, size_t> mBaseNum{
		{'T' , 0},
		{'C' , 1},
		{'A' , 2},
		{'G' , 3}
	};

	//State Machine to create amino acids
	char mStateMachine[24][4]
	{
		{0, 0, 1, 0}, //0
		{2, 0, 1, 0}, //1 , A**
		{0, 0, 1, 'M'}, //2 , AT*
		{4 , 9, 14, 19}, //3 , Next Codon  

		{5, 6, 7, 8}, //4 , T**
		//T    C    A    G 
		{'F', 'F', 'L', 'L'}, ///5 , TT*
		{'S', 'S', 'S', 'S'}, //6 , TC*
		{'Y', 'Y', 'x', 'x'}, //7, TA*
		{'C', 'C', 'x', 'W'}, //8 , TG*

		{10, 11, 12, 13}, //9 , C**
		//T    C    A    G
		{'L', 'L', 'L', 'L'}, //10 , CT*
		{'P', 'P', 'P', 'P'}, //11 , CC*
		{'H', 'H', 'Q', 'Q'}, //12 , CA*
		{'R', 'R', 'R', 'R'}, //13 , CG*

		{15, 16, 17, 18}, //14 , A**
		//T    C    A    G
		{'I', 'I', 'I', 'M'}, //15 , AT*
		{'T', 'T', 'T', 'T'}, //16 , AC*
		{'N', 'N', 'K', 'K'}, //17 , AA*
		{'S', 'S', 'R', 'R'}, //18 , AG*

		{20, 21, 22, 23}, //19 , G**
		//T    C    A    G
		{'V', 'V', 'V', 'V'}, //20 , GT*
		{'A', 'A', 'A', 'A'}, //21 , GC*
		{'D', 'D', 'E', 'E'}, //22 , GA*
		{'G', 'G', 'G', 'G'} //23 , GG*
	};
};