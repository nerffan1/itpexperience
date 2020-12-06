#pragma once
#include "MoveComponent.h"

class GoombaMove : public MoveComponent
{
public:
	GoombaMove(class Goomba* owner);
	void Update(float deltaTime);; 

	//Collision with other Actors
	void CheckActors(float deltaTime);

	//Useful Functions
	bool GoombaXNeighbourhood(class Actor* otherActor);
	bool GoombaYNeighbourhood(class Actor* otherActor);
	bool VerticalGoombaBoundary(class Block* blok);
	bool HorizontalGoombaBoundary(class Block* blok);
	void ChangeOtherGoombaSpeed(class Goomba* other);
protected:
	class Goomba* mGoombaOwner = nullptr;
	bool mInAir = true; //Start as true becasue they spawn on air
	float mYSpeed = 0.0f;

	//Useful variables to only check nearby objects
	float mXBoundary = 40.0f; 
	float mYBoundary = 16.0f;
	//Consts
	const float SPEED = 100;
	const float ACCEL = 2000.0f;

};