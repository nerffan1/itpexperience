#pragma once
#include <vector>
#include <string>
#include "Math.h"

class HeightMap
{
public:
	HeightMap();
	void LoadGridData(std::string filename);
	Vector3 CellToWorld(size_t row, size_t column);
	size_t* WorldToCell(float x, float y);
	bool IsOnTrack(float x, float y);
	float GetHeight(float x, float y);

private:
	std::vector<std::vector<int>> mGridValues;
	bool IsCellOnTrack(size_t row, size_t column);
	float GetHeightFromCell(size_t& row, size_t& column);

	//Constants
	float CELL_SIZE = 40.55f;
	float GRID_TOP = 1280.0f;
	float GRID_LEFT = -1641.0f;
};