#pragma once
#include <string>
#include <vector>
#include <random>

using DistancePair = std::pair<size_t, double>;
using ParentPair = std::pair<int, int>;
using Path = std::vector<int>;

struct Location
{
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

//Functions
Population CreatePopulation(int PopSize, std::mt19937& rando, int LocSize);
void PrintGeneration(Population& pop, int g);

//Fitness
DistancePair CalcFitness(size_t i, const std::vector<Location>& Locations, const std::vector<int>& member);
std::vector<Location> CreateLocations(const char* InputName);
std::vector<DistancePair> GetFitnessPairs(const std::vector<Location>& Locations, const std::vector<std::vector<int>>& pop);

//Selection
std::vector<ParentPair> GetParents(std::vector<DistancePair>& FitnessPairs, Population& Pop, std::mt19937& Rando);
ParentPair SelectParents(std::vector<DistancePair>& E, std::mt19937& Rando, std::ofstream& Output);
void PrintSelectionProb(std::vector<DistancePair>& E, const std::vector<DistancePair>& OrdFit, std::ofstream& Output);

//Crossover
void Beget(const std::vector<ParentPair>& Parents, Population& pop, std::mt19937& Rando, double mut);
std::vector<int> Infant(const int crox, const int geni, const ParentPair Parents,const Population& pop, std::mt19937& Rando, double mut);

//Solution
void PrintSolution(std::vector<DistancePair>& Fitness, std::vector<Location>& locs, Population& pop);