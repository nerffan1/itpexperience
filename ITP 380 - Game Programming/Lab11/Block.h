#pragma once
#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game, Actor* parent);
	void OnUpdate(float deltaTime) override;
	~Block();

	//Getter/Setter
	bool GetMirror() { return mMirror; }
	void SetMirror(bool mirr) { mMirror = mirr; }
	bool GetRot() { return mRot; }
	void SetRot(bool rot) { mRot = rot; }

protected:
	bool mMirror = false;
	bool mRot = false;
};