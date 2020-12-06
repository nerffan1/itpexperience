#pragma once
#include "VehicleMove.h"
#include <string>
#include "Math.h"
#include <vector>

class EnemyMove : public VehicleMove
{
public:
	EnemyMove(class Actor* owner);
	void Update(float deltaTime) override;
	void LoadPath(std::string filename);
	void AI();

private:
	std::vector<Vector3> mRoute;
	size_t mTarget = 1;
	const float WITHIN_VIEW = .999999f;
};