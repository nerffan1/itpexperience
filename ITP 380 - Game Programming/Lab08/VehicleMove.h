#pragma once
#include "Component.h"
#include "Math.h"
#include <string>
#include <vector>

enum steerDir
{
	right = 1,
	left = -1,
	none = 0
};
class VehicleMove : public Component
{
public:
	VehicleMove(class Actor* owmner);
	void Update(float deltaTime) override;

	//Movement Dynamics 
	Vector3 Acceleration(float& deltatime);
	void LinearMov(float& deltatime);
	void AngularMov(float& deltatime);

	//Setters
	void SetTurn(steerDir direction) { mDir = direction; }
	void SetAccel(bool pedal) { mAccel = pedal; }

	//Helper Functions
	Vector3 GetNormal();
	void LoadCheckPoints(std::string filename);
	void UpdateCheckPoint();

	//Laps/UI
	virtual void OnLapChange(int newLap) {};
	float DistanceToNextCp();

	int mCurrentLap = 0;
	int mLastCheckPoint = -1;

protected:
	bool mAccel = false;
	Vector3 mVel;
	float mAngVel;
	float mAccelTime = 0.0f;
	steerDir mDir = none;
	class HeightMap* mHeightMap = nullptr;
	std::vector<std::vector<size_t>> mCheckPointInfo;

	//Tunable Parameters
	const float MIN_ACCEL = 1000.0f;
	const float MAX_ACCEL = 2500.0f;
	const float RAMP_TIME = 2.0f;
	const float ANGULAR_ACCEL = 5 * Math::Pi;
	const float DRAG = 0.9f;
	const float DRAG_NOACCEL = 0.975f;
	const float ANGULAR_DRAG = 0.9f;
};