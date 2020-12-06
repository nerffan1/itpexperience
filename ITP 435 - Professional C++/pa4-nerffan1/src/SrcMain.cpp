#include "SrcMain.h"
#include <iostream>
#include "fasta.h"
#include "DNATrans.h"
#include "alignment.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	//argc == 2 --> argv[1] : FASTA file name
	//argc == 3 --> argv[1] / argv[2] : 2 FASTA files for aligning	

	//Determine Amino-Acid Count
	switch (argc)
	{
	case 2:
	{
		//Extract FASTA information
		std::string fastaFile = argv[1];
		fasta readAminos(fastaFile);

		//Translate Information and write to amino.txt file 
		DNATrans fastaTranslation(readAminos);
		break;
	}
	case 3:
	{
		//Filenames
		std::string fasta1 = argv[1];
		std::string fasta2 = argv[2];

		//Load fasta files
		fasta firstFile(fasta1);
		fasta secondFile(fasta2);

		//Align
		alignment mAlign(firstFile, secondFile);
		break;
	}

	}

}
