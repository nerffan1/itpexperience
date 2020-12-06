#include "fasta.h"

fasta::fasta(std::string fileName)
{
	//Open file at end to find size
	std::ifstream loadFasta(fileName, std::ios::ate);
	std::ifstream::pos_type fileSize;

	if (loadFasta.is_open())
	{
		//Load size
		fileSize = loadFasta.tellg();

		//Go back to beginning and get header
		loadFasta.clear();
		loadFasta.seekg(0, std::ios::beg);
		
		//Get header and remove > 
		std::getline(loadFasta, mHead);
		mHead.erase(mHead.begin());

		//read rest of file
		mSequence.reserve(fileSize);
		std::string tmp;
		tmp.reserve(70);
		while (!loadFasta.eof())
		{
			getline(loadFasta, tmp);
			mSequence += tmp;
		}
		mSequence.resize(mSequence.size());
	}
}