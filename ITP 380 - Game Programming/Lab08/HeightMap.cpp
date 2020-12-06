#include "HeightMap.h"
#include <fstream>
#include "CSVHelper.h"
#include"Game.h"

HeightMap::HeightMap()
{
	LoadGridData("Assets/HeightMap/HeightMap.csv");
}

void HeightMap::LoadGridData(std::string filename)
{
	std::ifstream HeightMapGrid;
	HeightMapGrid.open(filename);
	if (HeightMapGrid.is_open())
	{
		while (!HeightMapGrid.eof())
		{
			std::vector<std::string> rows;
			std::string line;
			std::getline(HeightMapGrid, line);
			rows = CSVHelper::Split(line);

			if (!line.empty())
			{
				//Convert strings to nums
				std::vector<int> ints;
				for (std::string num : rows)
				{
					ints.push_back(std::stoi(num));
				}
				mGridValues.push_back(ints);
			}
		}
	}
}

Vector3 HeightMap::CellToWorld(size_t row, size_t column)
{
	if (IsCellOnTrack(row, column))
	{
		Vector3 CellPos;
		CellPos.x = GRID_TOP - CELL_SIZE * row;
		CellPos.y = GRID_LEFT + CELL_SIZE * column;
		CellPos.z = GetHeightFromCell(row, column);
		return CellPos;
	}
	else
	{
		return Vector3::Zero;
	}
}

size_t* HeightMap::WorldToCell(float x, float y)
{
	size_t row = (size_t)((abs(x - GRID_TOP) + CELL_SIZE / 2.0f) / CELL_SIZE);
	size_t column = (size_t)((abs(y - GRID_LEFT) + CELL_SIZE / 2.0f) / CELL_SIZE);
	size_t* coor = new size_t [2];
	coor[0] = row;
	coor[1] = column;
	return coor;
}

bool HeightMap::IsOnTrack(float x, float y)
{
	size_t* grid = WorldToCell(x, y);
	if (IsCellOnTrack(grid[0], grid[1]))
	{
		return true;
	}
	else
	{
		return false;
	}
}

float HeightMap::GetHeight(float x, float y)
{
	if (IsOnTrack(x, y))
	{
		size_t* gridPair = WorldToCell(x, y);
		return GetHeightFromCell(gridPair[0], gridPair[1]);
	}
	else
	{
		return -1000.0f;
	}
}

bool HeightMap::IsCellOnTrack(size_t row, size_t column)
{
	if (mGridValues[row][column] != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float HeightMap::GetHeightFromCell(size_t& row, size_t& column)
{
	return  (mGridValues[row][column] * 5.0f) - 40.0f;
}
