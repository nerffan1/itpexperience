#pragma once
#include "Component.h"
#include "game.h"



class PointLight : public Component
{
public:
	PointLight(Game* pGame, RenderObj* pObj);
	~PointLight();

	//Override
	void LoadProperties(const rapidjson::Value& properties) override;
	void Update(float deltaTime) override;

protected:
	Game* mGame;
	PointLightData* mLData;
};