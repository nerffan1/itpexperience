#include "SrcMain.h"
#include <iostream>
#include <string>
#include <fstream>
#include "HashTable.hpp"
#include "sha1.h"
#include <algorithm>
#include <tbb/tbb.h>

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Implement

	std::string attack = argv[1];
	size_t tableSize{ static_cast<size_t>(std::stoi(argv[2])) };
	std::string inputFile{ argv[3] };
	std::string passwordFile{ argv[4] };
	HashTable<std::string> dicTable(tableSize);
	std::fstream keyFile;

	if (attack == "dictionary")
	{
		unsigned char hash[20];
		std::string hexStr;
		hexStr.resize(40);
		//Start HashTable
		//Read in a file and crate Hash Table
		std::fstream dicFile(inputFile);
		if (dicFile.is_open())
		{
			std::string tmp;
			while (!dicFile.eof())
			{
				std::getline(dicFile, tmp);
				sha1::Calc(tmp.c_str(), tmp.length(), hash);
				sha1::ToHexString(hash, &hexStr[0]);
				dicTable.Insert(hexStr, tmp);
			}
			dicFile.close();
			//Write a file that finds the password from the Hash Table
			std::fstream keyFile(passwordFile);
			std::ofstream dicOut("solved.txt");
			if (keyFile.is_open() && dicOut.is_open())
			{
				while (!keyFile.eof())
				{
					std::getline(keyFile, tmp);
					dicOut << tmp << ",";
					dicOut << *dicTable.Find(tmp) << "\n";;
				}
				dicOut.close();
				keyFile.close();
			}
		}
	}
	else if (attack == "phrases")
	{
		//Load file
		std::fstream nouns(inputFile);
		
		//Create vector
		std::vector<std::string> words;
		std::string tmp;
		if (nouns.is_open())
		{
			while (!nouns.eof())
			{
				std::getline(nouns, tmp);
				words.emplace_back(tmp);
			}
			nouns.close();
		}
		
		//Prep Table
		keyFile.open(passwordFile);
		std::vector<std::string> prepTable;
		if (keyFile.is_open())
		{
			while (!keyFile.eof())
			{
				std::getline(keyFile, tmp);
				dicTable.Insert(tmp, "");
				prepTable.emplace_back(tmp);
			}
			keyFile.close();
		}

		//Find permutations
		tmp.clear();
		size_t wordSize = words.size();
		tbb::parallel_for(tbb::blocked_range<size_t>(0, words.size()),
			[&dicTable, &words, &wordSize](tbb::blocked_range<size_t> w)
			{
				unsigned char hash[20];
				std::string hexStr;
				hexStr.resize(40);
				std::string a, b, c;
				std::string tmp2;

				for (size_t i = w.begin(); i < w.end(); ++i)
				{
					a = words[i];
					for (size_t j = 0; j < wordSize; j++)
					{
						b = words[j];
						for (size_t k = 0; k < wordSize; k++)
						{
							c = words[k];
							for (size_t l = 0; l < wordSize; l++)
							{
								tmp2 += a;
								tmp2 += b;
								tmp2 += c;
								tmp2 += words[l];

								//Calculate Sha-1 of permutation
								sha1::Calc(tmp2.c_str(), tmp2.length(), hash);
								sha1::ToHexString(hash, &hexStr[0]);
								if (dicTable.Find(hexStr) != nullptr)
								{
									*dicTable.Find(hexStr) = tmp2;
								}
								tmp2.clear();
							}
						}
					}
				}

			});
		std::ofstream dicOut("solved.txt");
		if (dicOut.is_open())
		{
			std::for_each(prepTable.begin(), prepTable.end(), [&](std::string& h)
				{
					dicOut << h << ",";
					dicOut << *dicTable.Find(h) << "\n"; 
				});
			dicOut.close();
		}
	}
}
