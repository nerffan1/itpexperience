#include "SrcMain.h"
#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO
	//[program] inputfile[1] popsize[2] generations[3] mutationchance[4] seed[5]
	//Save Parameters
	const char* inputFile{ argv[1] };
	int popSize{ std::stoi(argv[2]) };
	int generations{ std::stoi(argv[3]) };
	double mutChance{ std::stod(argv[4]) };
	std::mt19937 rando(std::stoi(argv[5]));
	std::ofstream Output("log.txt", std::ofstream::trunc);
	Output.close();

	//Define More Variables 
	std::vector<Location> locations = CreateLocations(inputFile);
	Population gen = CreatePopulation(popSize, rando, locations.size());
	std::vector<DistancePair> fitness;
	std::vector<ParentPair> parents;
	int i = 0;
	for (; i < generations; i++)
	{
		PrintGeneration(gen, i);
		//FitnerssVectors
		fitness = GetFitnessPairs(locations, gen.mMembers);
		//Create parents
		parents = GetParents(fitness, gen, rando);
		//Create NextGen Population 
		Beget(parents, gen, rando, mutChance);
	}
	PrintGeneration(gen, i);
	fitness = GetFitnessPairs(locations, gen.mMembers);
	PrintSolution(fitness, locations, gen);
}
