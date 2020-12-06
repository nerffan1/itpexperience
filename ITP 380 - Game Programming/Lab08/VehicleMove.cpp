#include "VehicleMove.h"
#include "Actor.h"
#include "MoveComponent.h"
#include "HeightMap.h"
#include "Game.h"
#include "CSVHelper.h"
#include <fstream>

VehicleMove::VehicleMove(Actor* owner) : Component(owner, 50)
{
	mHeightMap = mOwner->GetGame()->GetHeightMap();
	LoadCheckPoints("Assets/HeightMap/Checkpoints.csv");
}

void VehicleMove::Update(float deltaTime)
{
	if (mOwner->GetState() == ActorState::Active)
	{
		LinearMov(deltaTime);
		AngularMov(deltaTime);
		UpdateCheckPoint();
	}
}

void VehicleMove::AngularMov(float& deltatime)
{
	mAngVel += ANGULAR_ACCEL * deltatime * mDir;
	float currAngle = mOwner->GetRotation();
	mOwner->SetRotation(currAngle + mAngVel * deltatime);
	mAngVel *= ANGULAR_DRAG;
}

Vector3 VehicleMove::GetNormal()
{
	float KarAngle = mOwner->GetRotation();
	return Vector3(Math::Cos(KarAngle), Math::Sin(KarAngle), 0.0f);
}

void VehicleMove::LoadCheckPoints(std::string filename)
{
	std::ifstream CheckPoints;
	CheckPoints.open(filename);
	if (CheckPoints.is_open())
	{
		while (!CheckPoints.eof())
		{
			std::vector<std::string> rows;
			std::string line;
			std::getline(CheckPoints, line);
			rows = CSVHelper::Split(line);

			if (!line.empty() && (rows[0] == "Checkpoint"))
			{
				//Convert strings to size_t
				std::vector<size_t> cells;
				for (size_t i = 1; i < 5; i++)
				{
					size_t cellCoor = (size_t)std::stoi(rows[i]);
					cells.push_back(cellCoor);
				}
				mCheckPointInfo.push_back(cells);
			}
		}
	}
}

void VehicleMove::UpdateCheckPoint()
{
	//Load the points to loop through
	size_t NextCp = (size_t)mLastCheckPoint + 1;
	size_t MinX = mCheckPointInfo[NextCp][0];
	size_t MaxX = mCheckPointInfo[NextCp][1];
	size_t MinY = mCheckPointInfo[NextCp][2];
	size_t MaxY = mCheckPointInfo[NextCp][3];
	float x = mOwner->GetPosition().x;
	float y = mOwner->GetPosition().y;
	size_t* currCells = mHeightMap->WorldToCell(x, y);

	//Loop through the checkpoint lines
	if (MinX == MaxX)
	{
		size_t Index = MinY;
		while (Index != (MaxY + 1)) //Add one so that last value is also compared
		{
			if (currCells[0] == MinX && currCells[1] == Index)
			{
				mLastCheckPoint++;
				//Wrap around if all checkpoints have been met
				if (mLastCheckPoint == mCheckPointInfo.size() - 1)
				{
					mLastCheckPoint = -1;
				}
				//Update Lap
				if (mLastCheckPoint == 0)
				{
					mCurrentLap++;
					OnLapChange(mCurrentLap);
				}
			}
			Index++;
		}
	}
	else if (MinY == MaxY)
	{
		size_t Index = MinX;
		while (Index != (MaxX + 1)) //Add one so that last value is also compared
		{
			if (currCells[0] == Index && currCells[1] == MinY)
			{
				mLastCheckPoint++;
				//Wrap around if all checkpoints have been met
				if (mLastCheckPoint == mCheckPointInfo.size() - 1)
				{
					mLastCheckPoint = -1;
				}
				//Update Lap
				if (mLastCheckPoint == 0)
				{
					mCurrentLap++;
				}
			}
			Index++;
		}
	}
}

float VehicleMove::DistanceToNextCp()
{
	Vector3 PlayePos = mOwner->GetPosition();
	size_t nextCpX = mCheckPointInfo[mLastCheckPoint + 1][0];
	size_t nextCpY = mCheckPointInfo[mLastCheckPoint + 1][3];
	Vector3 NextCpCoor = mHeightMap->CellToWorld(nextCpX, nextCpY);
	return (NextCpCoor - PlayePos).Length();
}

Vector3 VehicleMove::Acceleration(float& deltatime)
{
	if (mAccel == true)
	{
		mAccelTime += deltatime;
		float KarAngle = mOwner->GetRotation();
		Vector3 normal(Math::Cos(KarAngle), Math::Sin(KarAngle), 0.0f);

		//Return acceleration with direction
		if (mAccelTime >= 0.0f && mAccelTime <= 2.0f)
		{
			float magnitude = Math::Lerp(MIN_ACCEL, MAX_ACCEL, mAccelTime / 0.4);
			return normal * magnitude;
		}
		else
		{
			float magnitude = Math::Lerp(MIN_ACCEL, MAX_ACCEL, 1.0f);
			return normal * magnitude;
		}
	}
	else
	{
		mAccelTime = 0;
		return Vector3::Zero;
	}
}

void VehicleMove::LinearMov(float& deltatime)
{
	//Update Velocity
	mVel += Acceleration(deltatime) * deltatime;

	//Update Position
	Vector3 dl = mVel * deltatime;
	Vector3 currentPos = mOwner->GetPosition();
	mOwner->SetPosition(currentPos + dl);

	//Update Z-Axis
	float x = mOwner->GetPosition().x;
	float y = mOwner->GetPosition().y;

	if (mHeightMap->IsOnTrack(x, y))
	{
		Vector3 currPos = mOwner->GetPosition();
		float currZ = currPos.z;
		float nextZ = mHeightMap->GetHeight(x, y);
		currPos.z = Math::Lerp(currZ, nextZ, 0.1f);
		mOwner->SetPosition(currPos);
	}

	//Apply Drag
	if (mAccel)
	{
		mVel *= DRAG;
	}
	else
	{
		mVel *= DRAG_NOACCEL;
	}
}
