#pragma once
#include "UIComponent.h"
#include <string>

class HUD : public UIComponent
{
public:
	HUD(class Actor* owner);
	~HUD();
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	std::string GetTime();
	void AddCoin();

	//CheckPoint Display
	void SetCPText(class Texture* t) { mCPText = t; }
	void CheckpointText(class Shader* shader);
	void SetCPColl(bool coll);
	void SetCPString(std::string s) { mCPDisplay = s; }
	float GetCPTimer() { return mCPTimer; }

	//Radar 
	void DrawCameras(class Shader* shader);

private:
	class Font* mFont = nullptr;
	class Texture* mTimerText = nullptr;
	class Texture* mCoinText = nullptr;
	class Texture* mCPText = nullptr;
	class Texture* mRadar = nullptr;
	class Texture* mRadarArrow = nullptr;
	class Texture* mBlip = nullptr;

	
	//String for CP
	std::string mCPDisplay = "";

	//Timer
	bool mCPColl = false;
	float mCPTimer = 0.0f;
	const float CP_TIME = 5.0f;

	//Radar
	const Vector2 RADAR_POS = Vector2(400.0f, -275.0f);
	const Vector2 PLAYER_FORWARD = Vector2(0.0f, 1.0f);
	const float RADAR_RANGE = 1500.0f;
	const float RADAR_RAD = 92.0f;
};