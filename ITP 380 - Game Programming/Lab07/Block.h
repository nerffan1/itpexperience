#pragma once
#include "Actor.h"
#include "Mesh.h"
#include "MeshComponent.h"

enum type
{
	exploding,
	normal
};

class Block : public Actor
{
public:
	Block(class Game* game, Vector3 pos, char type, int layer);
	~Block();
	void OnUpdate(float deltaTime);
	void Explode();
	type mType;
	int mLayer;

protected:
	class MeshComponent* mMesh = nullptr;
	
	//Constant
	const float DeleteCoor = 450.0f;
};