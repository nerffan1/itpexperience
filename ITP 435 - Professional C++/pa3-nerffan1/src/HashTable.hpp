#pragma once
#include <string>
#include <utility>
#include <functional>

template <typename ValueType,
	typename HashFuncType = std::hash<std::string>>
	class HashTable
{
public:
	// Constructs the hash table with the given capacity (size of underlying array)
	HashTable(size_t capacity)
		:mTableData(new std::pair<std::string, ValueType>[capacity])
		, mCapacity(capacity)
		, mSize(0)
	{ }

	// TODO: Implement "Rule of five" functions
	HashTable() = delete;
	~HashTable()
	{
		delete[] mTableData;
		mTableData = nullptr;
		mSize = 0;
		mCapacity = 0;
	}

	//Copy Constructor - Deep Copy
	HashTable(const HashTable& rhs) :
		mCapacity(rhs.mCapacity),
		mSize(rhs.mSize),
		mHashFunc(rhs.mHashFunc)
	{
		mTableData = new std::pair<std::string, ValueType>[mCapacity];
		for (size_t i = 0; i < mCapacity; i++)
		{
			mTableData[i].first = rhs.mTableData[i].first;
			mTableData[i].second = rhs.mTableData[i].second;
		}
	}
	//Move Constructor
	HashTable(HashTable&& rhs) noexcept :
		mTableData(std::move(rhs.mTableData)),
		mCapacity(std::move(rhs.mCapacity)),
		mSize(std::move(rhs.mSize)),
		mHashFunc(std::move(rhs.mHashFunc))
	{
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mTableData = nullptr;
	}
	//Copy Assignment
	HashTable& operator=(const HashTable& rhs)
	{
		if (this != &rhs)
		{
			//Delete data
			delete[] mTableData;
			//Deep Copy
			mCapacity = rhs.mCapacity;
			mSize = rhs.mSize;

			mTableData = new std::pair<std::string, ValueType>[mCapacity];
			for (size_t i = 0; i < mCapacity; i++)
			{
				mTableData[i] = rhs.mTableData[i];
			}
		}
		//Return this
		return *this;
	}
	//Move Assignment
	HashTable& operator=(HashTable&& rhs) noexcept
	{
		if (this != &rhs)
		{
			//Copy
			mCapacity = std::move(rhs.mCapacity);
			mSize = std::move(rhs.mSize);
			delete[] mTableData;
			mTableData = rhs.mTableData;
			//Clean Old Data
			rhs.mCapacity = 0;
			rhs.mSize = 0;
			rhs.mTableData = nullptr;
			//Return this
		}
		return *this;
	}

	// Returns overall capacity of hash table (size of underlying array)
	size_t Capacity() const
	{
		return mCapacity;
	}

	// Returns the number of elements currently inserted in the hash table
	size_t Size() const
	{
		return mSize;
	}

	// Tries to insert (key, value) into hash table
	// Returns true if insert succeeds, false if not
	bool Insert(const std::string& key, const ValueType& value)
	{
		if (mSize == mCapacity)
		{
			return false;
		}
		//Determine Index
		size_t index = mHashFunc(key) % mCapacity;
		if (mTableData[index].first.empty())
		{
			mTableData[index].first = key;
			mTableData[index].second = value;
			mSize++;
			return true;
		}

		while (!mTableData[index].first.empty())
		{
			++index;
			if (index != mCapacity)
			{
				if (mTableData[index].first.empty())
				{
					mTableData[index].first = key;
					mTableData[index].second = value;
					mSize++;
					return true;
				}
			}
			else //Wrap around to 0 
			{
				if (mTableData[0].first.empty())
				{
					mTableData[0].first = key;
					mTableData[0].second = value;
					mSize++;
					return true;
				}
				index = 0;

			}
		}
		return false;
	}

	// Returns pointer to value for given key, or nullptr
	// if not in hash table
	ValueType* Find(const std::string& key)
	{
		// TODO: Implement
		size_t index = mHashFunc(key) % mCapacity;
		size_t notinloop = index;
		if (mTableData[index].first.empty())
		{
			return nullptr;
		}
		if (mTableData[index].first == key)
		{
			return &mTableData[index].second;
		}
		while (!mTableData[index].first.empty())
		{
			++index;
			if (index == notinloop) //If we return to same value
			{
				break;
			}
			if (index != mCapacity)
			{
				if (mTableData[index].first.empty())
				{
					return nullptr;
				}
				if (mTableData[index].first == key)
				{
					return &mTableData[index].second;
				}

			}
			else //Wrap around to 0 
			{
				if (mTableData[0].first.empty())
				{
					return nullptr;
				}
				if (mTableData[0].first == key)
				{
					return &mTableData[0].second;
				}
				index = 0;
				if (index == notinloop) 
				{
					break;
				}
			}
		}
		return nullptr;
	}

	// Executes provided lambda expression on each element inserted into the table
	void ForEach(std::function<void(const std::string& key, ValueType& value)> func)
	{
		// TODO: Implement
		for (size_t i = 0; i < mCapacity; i++)
		{
			if (!mTableData[i].first.empty())
			{
				func(mTableData[i].first, mTableData[i].second);
			}
		}
	}


	// Gets a const pointer to underlying data (for testing)
	const std::pair<std::string, ValueType>* Data() const
	{
		return mTableData;
	}
private:
	// NOTE: you should not need any more member data!

	// Underlying hash table array
	std::pair<std::string, ValueType>* mTableData;
	// Allocated capacity of hash table array
	size_t mCapacity;
	// Number of elements in HashTable
	size_t mSize;
	// Hash function
	HashFuncType mHashFunc;
};
