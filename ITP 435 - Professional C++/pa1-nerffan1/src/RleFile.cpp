#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

void RleFile::CreateArchive(const std::string& source)
{
	//Open file in binary mode
	std::ifstream::pos_type size;
	char* memblock; // Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();
		// File data is now in memblock array
		//Write data to Header
		mHeader.mFileName = source;
		mHeader.mFileNameLength = static_cast<int>(source.size());
		mHeader.mFileSize = size;
		mRleData.Compress(memblock, size);  // Minus 1?

		//Write Compressed data
		std::ofstream arc(source + ".rl1", std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open())
		{
			// Use arc.write function to write data here
			//Write Signature.
			arc.write(mHeader.mSig, 4);
			//Write Filesize
			arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)), 4);
			//Write Filname Length
			arc.write(reinterpret_cast<char*>(&(mHeader.mFileNameLength)), 1);
			//Write Filename
			arc.write(mHeader.mFileName.c_str(), mHeader.mFileNameLength);
			//Write Compressed Data
			arc.write(mRleData.mData, mRleData.mSize);

			arc.close();
		}
		// Make sure to clean up!
		delete[] memblock;
	}
}

void RleFile::ExtractArchive(const std::string& source)
{
	// TODO
	//Open file in binary mode
	std::ifstream::pos_type size;
	char* memblock; // Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();

		// File data is now in memblock array. Write data to Header
		mHeader.mFileSize = *(reinterpret_cast<int*>(&memblock[4]));
		mHeader.mFileNameLength = *(reinterpret_cast<int*>(&memblock[8]));
		
		//Extract Name
		for (size_t i = 0; i < mHeader.mFileNameLength; i++)
		{
			mHeader.mFileName += *(reinterpret_cast<char*>(&memblock[9 + i]));
		}

		//Compress Data 
		size_t comprSz { static_cast<size_t>(size) - (9 + static_cast<size_t>(mHeader.mFileNameLength)) };
		mRleData.Decompress(&memblock[9 + mHeader.mFileNameLength], comprSz, mHeader.mFileSize);

		//WriteData
		std::ofstream arc(mHeader.mFileName, std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open())
		{
			// Use arc.write function to write data here
			//Write Signature.
			arc.write(mRleData.mData, mHeader.mFileSize);
			arc.close();
		}

		// Make sure to clean up!
		delete[] memblock;
	}
}
