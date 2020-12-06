#pragma once
#include <string>
#include <vector>
#include <tuple>

inline std::vector<std::string> Split(const std::string& str)
{
	const char delim = ',';
	std::vector<std::string> retVal;

	size_t start = 0;
	size_t delimLoc = str.find_first_of(delim, start);
	while (delimLoc != std::string::npos)
	{
		retVal.emplace_back(str.substr(start, delimLoc - start));

		start = delimLoc + 1;
		delimLoc = str.find_first_of(delim, start);
	}

	retVal.emplace_back(str.substr(start));
	return retVal;
}

// Parse Element 
template<typename T>
inline std::tuple<T> ParseElem(const std::string& elem) {}

template<>
inline std::tuple<int> ParseElem<int>(const std::string& elem)
{
	return std::make_tuple(std::stoi(elem));
}

template<>
inline std::tuple<std::string> ParseElem<std::string>(const std::string& elem)
{
	return std::make_tuple(elem);
}

//Forward Declaration
template<typename T, typename... Args>
std::tuple<T, Args...> ParseStrHelper(std::vector<std::string>& paramV);

//Function: ParseStr
template <typename... Args>
std::tuple<Args...> ParseStr(std::vector<std::string>& paramV)
{
	return ParseStrHelper<Args...>(paramV);
}

template <>
inline std::tuple<> ParseStr(std::vector<std::string>& paramV)
{
	return std::tuple();
}

template <typename T, typename... Args>
std::tuple<T, Args...> ParseStrHelper(std::vector<std::string>& paramV)
{
	std::string elem = paramV.back();
	paramV.pop_back();
	//ParseElem gives a tuple of the type passed.
	//ParseStr
	return std::tuple_cat(ParseElem<T>(elem), ParseStr<Args...>(paramV));
}
