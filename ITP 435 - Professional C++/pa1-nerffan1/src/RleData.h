#pragma once
#include <ostream>

// RLE Compression/Decompression
struct RleData
{
	// Memory which stores either compressed or decompressed data
	char* mData;
	const char* Inputptr = nullptr;
	// Number of elements of type T that data is pointing to
	int mSize;

	//Write Aid
	size_t LastElement{ 0 };
	size_t CompLastRead{ 0 };

	RleData()
	: mData(nullptr)
	, mSize(0)
	{ }

	~RleData()
	{
		delete[] mData;
	}

	// Delete other five
	RleData(const RleData& other) = delete;
	RleData(RleData&& other) = delete;
	RleData& operator=(const RleData& other) = delete;
	RleData& operator=(RleData&& other) = delete;
	
	// Compresses input data and stores it in mData
	void Compress(const char* input, size_t inSize);

	// Decompresses input data and stores it in mData
	void Decompress(const char* input, size_t inSize, size_t outSize);

	// Outputs mData
	friend std::ostream& operator<< (std::ostream& stream, const RleData& rhs);

	//Additional Functions
	void CompressWrite(size_t beg, const size_t& end, const bool& sign);
	void InsertData(char in);
	void DecompressWrite(size_t& inSize, size_t& Readbeg, size_t& WriteBeg );

	size_t MaxRunSize()
	{
		// Bitwise magic
		return 127;
	}
};

