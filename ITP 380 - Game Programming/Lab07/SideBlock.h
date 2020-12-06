#pragma once
#include "Actor.h"
#include <string>

class SideBlock : public Actor
{
public:
	SideBlock(class Game* game, int ColorIndex);
	~SideBlock();
	void OnUpdate(float deltaTime) override;
protected:
	class MeshComponent* mMesh = nullptr;
	//Constant
	const float DeleteCoor = 450.0f;
};