#include "TiledBGComponent.h"
#include "Actor.h"
#include "Game.h"
#include <fstream>
#include <string>
#include "CSVHelper.h"

TiledBGComponent::TiledBGComponent(Actor* mOwner, int drawOrder)
	:SpriteComponent(mOwner, drawOrder) 
	,mTileHeight(32) 
	,mTileWidth(32)
{
	SetIsVisible(true);
	LoadTileCSV("Assets/Dungeon/DungeonMapBG.csv", 32, 32);
	SetTexture(mOwner->GetGame()->GetTexture(("Assets/Dungeon/DungeonTiles.png")));
}

void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight)
{
	std::ifstream CSVFile;
	CSVFile.open(fileName);

	if (CSVFile.is_open())
	{
		std::string line;

		//Iterate through CSV, and make a vector of vectors with those
		while (!CSVFile.eof())
		{
			//Retrieve the numbers of the first row in form of a string, which will be turned into ints
			std::vector<std::string> columnsS;
			std::vector<int> columnsI;

			//Do not process empty lines
			std::getline(CSVFile, line);
			if (!line.empty())
			{
				//Turn line into a vector 
				columnsS = CSVHelper::Split(line);

				//Turn the data into a vector of integers and push it to our member variable
				for (size_t j = 0; j < columnsS.size(); j++)
				{
					columnsI.push_back(std::stoi(columnsS[j]));
				}
				mCSVInfo.push_back(columnsI);
			}
		}
	}
	else
	{
		SDL_Log("Unable to open %s", fileName);
	}
}

void TiledBGComponent::Draw(SDL_Renderer* renderer)
{
	//Loop through CSVInfo to draw tiles 
	for (size_t i = 0; i < mCSVInfo.size(); i++)
	{
		for (size_t j = 0 ; j < mCSVInfo[i].size() ; j++)
		{
			if (mCSVInfo[i][j] != -1)
			{
				if (mTexture)
				{
					SDL_Rect r;
					r.w = static_cast<int>(mTileWidth);
					r.h = static_cast<int>(mTileHeight);
					r.x = static_cast<int>((32*j) - mOwner->GetGame()->GetCamera().x);
					r.y = static_cast<int>((32*i) - mOwner->GetGame()->GetCamera().y);

					// Draw (have to convert angle from radians to degrees, and clockwise to counter)
					SDL_RenderCopyEx(renderer,
						mTexture,
						GetSourceRect(mCSVInfo[i][j]),
						&r,
						0.0,
						nullptr,
						SDL_FLIP_NONE);
				}
			}
		}
	}
}

SDL_Rect* TiledBGComponent::GetSourceRect(int tile) const
{
	//Variables
	int column;
	int row;
	
	//Find the row and column
	column = tile % 13;
	row = tile / 13;

	// Create source rectangle
	r->w = mTileWidth;
	r->h = static_cast<int>(mTileHeight);
	r->x = static_cast<int>((32*column));
	r->y = static_cast<int>((32*row));
	return r;
}
