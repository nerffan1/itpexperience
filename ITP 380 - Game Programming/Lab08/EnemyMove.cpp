#include "EnemyMove.h"
#include "Actor.h"
#include <fstream>
#include <string>
#include "Game.h"
#include "CSVHelper.h"
#include "HeightMap.h"

EnemyMove::EnemyMove(Actor* owner) : VehicleMove(owner)
{
	LoadPath("Assets/HeightMap/Path.csv");
	mOwner->SetPosition(mRoute[0]);
}

void EnemyMove::Update(float deltaTime)
{
	AI();
	VehicleMove::Update(deltaTime);
}

void EnemyMove::LoadPath(std::string filename)
{
	std::ifstream PathInfo;
	PathInfo.open(filename);
	if (PathInfo.is_open())
	{
		while (!PathInfo.eof())
		{
			std::vector<std::string> rows;
			std::string line;
			std::getline(PathInfo, line);
			rows = CSVHelper::Split(line);

			if (!line.empty() && rows[0] == "Node")
			{
				size_t xCell = (size_t)std::stoi(rows[1]);
				size_t yCell = (size_t)std::stoi(rows[2]);
				Vector3 pos = mOwner->GetGame()->GetHeightMap()->CellToWorld(xCell, yCell);
				mRoute.push_back(pos);
			}
		}
	}
}

void EnemyMove::AI()
{
	//Check if target is roughly within 15 degrees of view 
	float KarToTarMag = (mRoute[mTarget] - mOwner->GetPosition()).Length();
	Vector3 KarToTar = (mRoute[mTarget] - mOwner->GetPosition()) * (1/ KarToTarMag);
	float DotTarnorm = Math::Abs(Vector3::Dot(GetNormal(), KarToTar));
	if ( DotTarnorm > WITHIN_VIEW)
	{
		mAccel = true;
	}
	else
	{
		//Find out which way to turn
		float CrossProduct = Vector3::Cross(GetNormal(), KarToTar).z;
		
		if (CrossProduct > 0)
		{
			mDir = steerDir::right;
		}
		else
		{
			mDir = steerDir::left;
		}
	}
	
	//Check how close you are to the target Node, and change it
	float DistToTar = (mRoute[mTarget] - mOwner->GetPosition()).Length();
	if (DistToTar < 95.0f)
	{
		mTarget++;
		if (mTarget > 27)
		{
			mTarget = 0;
		}
	}
}