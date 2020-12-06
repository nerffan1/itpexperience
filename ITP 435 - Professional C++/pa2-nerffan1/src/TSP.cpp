#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>

Population CreatePopulation(int PopSize, std::mt19937& Rando, int LocSize)
{
	Population newPop;
	newPop.mMembers.resize(PopSize);
	int j = -1;
	std::generate(newPop.mMembers.begin(), newPop.mMembers.end(),
		[&]()
		{
			++j;
			newPop.mMembers[j].resize(LocSize);
			int i = 0;
			std::generate(
				newPop.mMembers[j].begin(),
				newPop.mMembers[j].end(), [&]()
				{
					return i++;
				}
			);
			std::shuffle(newPop.mMembers[j].begin() + 1, newPop.mMembers[j].end(), Rando);
			return newPop.mMembers[j];
		}
	);
	return newPop;
}

void PrintGeneration(Population& pop, int g)
{
	std::ofstream Outfile("log.txt", std::fstream::app);
	if (Outfile.is_open())
	{
		if (g == 0)
		{
			Outfile << "INITIAL POPULATION:\n";
		}
		else
		{
			Outfile << "GENERATION: " << g << "\n";
		}
		for (auto member : pop.mMembers)
		{
			for (auto ints : member)
			{
				Outfile << ints;

				if (ints != *(member.end() - 1))
				{
					Outfile << ",";
				}
			}
			Outfile << "\n";
		}
	}
	Outfile.close();
}

DistancePair CalcFitness(size_t i, const std::vector<Location>& locations, const std::vector<int>& member)
{
	//Variables
	double lon1, lon2, lat1, lat2, dlon, dlat;
	double a, c;
	double fitNum = 0;
	int lastStop = member[member.size() - 1];
	std::accumulate(member.begin(), member.end(), lastStop,
		[&](const int& strt, const int& end)
		{
			//Extract values
			lon1 = locations[static_cast<size_t>(strt)].mLongitude;
			lon2 = locations[static_cast<size_t>(end)].mLongitude;
			lat1 = locations[static_cast<size_t>(strt)].mLatitude;
			lat2 = locations[static_cast<size_t>(end)].mLatitude;
			//Calculate Haversine
			dlon = lon2 - lon1;
			dlat = lat2 - lat1;
			a = pow(sin(dlat / 2), 2);
			a += cos(lat1) * cos(lat2) * pow((sin(dlon / 2)), 2);
			c = 2 * atan2(sqrt(a), sqrt(1 - a));
			fitNum += c * 3961;
			return end;
		}
	);

	//Write to Log
	std::ofstream output("log.txt", std::ofstream::app);
	if (output.is_open())
	{
		output << i << ":" << fitNum << "\n";
		output.close();
	}

	return std::make_pair(i, fitNum);
}

std::vector<Location> CreateLocations(const char* InputName)
{
	std::vector<Location> locations;
	std::ifstream locFile(InputName);
	if (locFile.is_open())
	{
		Location insertLoc;
		std::string stringtmp;
		while (!locFile.eof())
		{
			//Name
			std::getline(locFile, stringtmp, ',');
			insertLoc.mName = stringtmp;
			//Latitude
			std::getline(locFile, stringtmp, ',');
			insertLoc.mLatitude = std::stod(stringtmp);
			insertLoc.mLatitude *= 0.0174533;
			//Longitude
			std::getline(locFile, stringtmp, '\n');
			insertLoc.mLongitude = std::stod(stringtmp);
			insertLoc.mLongitude *= 0.0174533;
			locations.emplace_back(insertLoc);
		}
	}
	return locations;
}

std::vector<DistancePair> GetFitnessPairs(const std::vector<Location>& Locations, const std::vector<std::vector<int>>& pop)
{
	std::vector<DistancePair> pairs;
	pairs.resize(pop.size());
	int i = -1;

	//Print Title
	std::ofstream Output("log.txt", std::ofstream::app);
	if (Output.is_open())
	{
		Output << "FITNESS:" << "\n";
		Output.close();
	}
	//Generate Population
	std::generate(pairs.begin(), pairs.end(), [&]()
		{
			++i;
			return CalcFitness(i, Locations, pop[i]);
		});
	return pairs;
}

std::vector<ParentPair> GetParents(std::vector<DistancePair>& FitnessPairs, Population& Pop, std::mt19937& Rando)
{
	int popSize = static_cast<int>(Pop.mMembers.size());
	//Sort Parents
	std::sort(FitnessPairs.begin(), FitnessPairs.end(), [&](DistancePair& left, DistancePair& right)
		{
			return left.second < right.second;
		}
	);
	//**********************************************************************************************
	//Probabilities
	std::vector<DistancePair> expect(popSize);
	std::transform(expect.begin(), expect.end(), FitnessPairs.begin(), expect.begin(),
		[popSize](DistancePair& Prob, DistancePair& Ordered)
		{
			return DistancePair(Ordered.first, (1.0 / popSize)); //Maintain first index as 0
		});

	//Multiply by 6 the top 2  
	expect[0].second *= 6;
	expect[1].second *= 6;
	//Multiply rest of half by 3
	int mid = (popSize / 2) - 1;
	for (size_t i = 2; i <= static_cast<size_t>(mid); i++)
	{
		expect[i].second *= 3;
	}
	//Create normalization factor 
	double norm = 0;
	DistancePair tmp;
	std::accumulate(expect.begin(), expect.end(), tmp,
		[&norm](DistancePair A, DistancePair& B)
		{
			norm += B.second;
			return B;
		});
	//Normalize
	std::for_each(expect.begin(), expect.end(), [&norm](DistancePair& P)
		{
			P.second *= (1.0 / norm);
		});
	//Print
	std::ofstream output;
	PrintSelectionProb(expect, FitnessPairs, output);
	//***************************************************************************************************

	//Create Random Selection
	std::vector<ParentPair> padres(popSize);
	std::generate(padres.begin(), padres.end(), [&]()
		{
			return SelectParents(expect, Rando, output);
		}
	);
	//Print pairs
	output.open("log.txt", std::fstream::app);
	if (output.is_open())
	{
		output << "SELECTED PAIRS:" << "\n";
		for (auto j : padres)
		{
			output << "(" << j.first << "," << j.second << ")";
			output << "\n";
		}
		output.close();
	}
	return padres;
}

ParentPair SelectParents(std::vector<DistancePair>& E, std::mt19937& Rando, std::ofstream& Output)
{

	//Random numbers in range
	std::uniform_real_distribution<double> distribution(0, 1);
	double dad = distribution(Rando);
	double mom = distribution(Rando);
	//Print Random pairs
	/*Output.open("log.txt", std::fstream::app);
	if (Output.is_open())
	{
		Output << "Random Real Pair: (" << dad << " " << mom << " )\n";
		Output.close();
	}*/
	int momi{ -1 };
	int dadi{ -1 };
	for (int i = 0; i <= E.size(); i++)
	{
		double a = 0;
		DistancePair temp;
		std::accumulate(E.begin(), E.begin() + i, temp,
			[&a](DistancePair A, DistancePair& B)
			{
				a += B.second;
				return B;
			});
		if (a >= mom && momi == -1)
		{
			momi = i - 1;
		}
		if (a >= dad && dadi == -1)
		{
			dadi = i - 1;
		}
		if (momi != -1 && dadi != -1)
		{
			break;
		}
	}
	return ParentPair(dadi, momi);
}

void PrintSelectionProb(std::vector<DistancePair>& E, const std::vector<DistancePair>& OrdFit, std::ofstream& Output)
{
	//Sort Expectations 
	std::sort(E.begin(), E.end(), [&](DistancePair& left, DistancePair& right)
		{
			return left.first < right.first;
		}
	);
	/*Output.open("log.txt", std::fstream::app);
	if (Output.is_open())
	{

		Output << "Selection Probabilities \n";
		for (size_t i = 0; i < E.size(); i++)
		{
			Output << i << ": " << E[i].second << "\n";;
		}
	}
	Output.close();*/
}

void Beget(const std::vector<ParentPair>& Parents, Population& pop, std::mt19937& Rando, double mut)
{
	size_t popsize = pop.mMembers.size();
	Population New;
	New.mMembers.resize(popsize);

	std::transform(Parents.begin(), Parents.end(), pop.mMembers.begin(), New.mMembers.begin(),
		[&](const ParentPair& pre, Path& locs)
		{
			int range = static_cast<int>(pop.mMembers[0].size());
			std::uniform_int_distribution<int> dist1(1, range - 2);
			int cindex = dist1(Rando);
			std::uniform_int_distribution<int> dist2(0, 1);
			int geni = dist2(Rando);
			return Infant(cindex, geni, pre, pop, Rando, mut);
		}
	);
	pop = New;
}

std::vector<int> Infant(const int crox, const int geni, const ParentPair Parents, const Population& pop, std::mt19937& Rando, double mut)
{
	const Path* FirstParent;
	const Path* SecondParent;
	if (geni == 0)
	{
		FirstParent = &pop.mMembers[Parents.second];
		SecondParent = &pop.mMembers[Parents.first];
	}
	else
	{
		FirstParent = &pop.mMembers[Parents.first];
		SecondParent = &pop.mMembers[Parents.second];
	}
	Path newBorn;
	newBorn.resize(FirstParent->size());
	std::copy_n(FirstParent->begin(), crox + 1, newBorn.begin());
	Path missing(newBorn.size());
	//Find missing values
	std::copy_if(SecondParent->begin(), SecondParent->end(), missing.begin(),
		[&newBorn](int num)
		{
			if (std::find(newBorn.begin(), newBorn.end(), num) == newBorn.end())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	);
	std::copy_n(missing.begin(), newBorn.size() - crox - 1, newBorn.begin() + crox + 1);

	////Create Mutation
	std::uniform_real_distribution<double> dist3(0, 100);
	double change = dist3(Rando);
	change /= 100;
	bool mutate = change <= (mut / 100);
	int index1;
	int index2;
	if (mutate)
	{
		int range = static_cast<int>(newBorn.size() - 1);
		std::uniform_int_distribution<int> mutDist(1, range);
		index1 = mutDist(Rando);
		index2 = mutDist(Rando);
		std::swap(newBorn[index1], newBorn[index2]);
	}

	//////***********************************************************************************************************

	//Print Information
	/*std::ofstream output("log.txt", std::fstream::app);
	if (output.is_open())
	{
		output << "----------------------------\n";
		output << "Crossover for pair (" << Parents.first << "," << Parents.second << ")\n";
		output << "Crossover Index: " << crox << "\n";
		output << "Coin Result: " << geni << "\n";
		output << "Mutation Random Real: " << change << "\n";
		if (mutate)
		{
			output << "Mutating Pair: (" << index1 << "," << index2 << ")\n";
		}
		else
		{
			output << "Not Mutating\n";
		}
		output.close();
	}*/
	return newBorn;
}

void PrintSolution(std::vector<DistancePair>& Fitness, std::vector<Location>& locs, Population& pop)
{
	std::ofstream output("log.txt", std::ofstream::app);
	//Find shortest
	DistancePair winner = Fitness[0];
	std::for_each(Fitness.begin(), Fitness.end(),
		[&winner](DistancePair& p)
		{
			if (winner.second > p.second)
			{
				winner = p;
			}
		}
	);


	if (output.is_open())
	{
		output << "SOLUTION:\n";
		for (auto index : pop.mMembers[winner.first])
		{
			output << locs[index].mName << "\n";
		}
		output << locs[0].mName << "\n";
		output << "DISTANCE: " << winner.second << " miles";
		output.close();
	}
}
