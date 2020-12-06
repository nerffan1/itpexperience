#include "HUD.h"
#include "SDL/SDL_ttf.h"
#include "Font.h"
#include "Texture.h"
#include <string>
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "SecurityCamera.h"

HUD::HUD(Actor* owner) : UIComponent(owner)
{
	mFont = new Font;
	mFont->Load("Assets/Inconsolata-Regular.ttf");
	mTimerText = mFont->RenderText(GetTime());
	mCPText = mFont->RenderText("");
	mCoinText = mFont->RenderText(std::to_string(mOwner->GetGame()->GetCoin()) + "/55");

	//Load Radar
	mRadar = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
	mRadarArrow = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
	mBlip = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");
}

HUD::~HUD()
{
	mFont->Unload();
	delete mFont;
}

void HUD::Update(float deltaTime)
{
	mOwner->GetGame()->AddToTime(deltaTime);
	mTimerText->Unload();
	delete mTimerText;
	mTimerText = mFont->RenderText(GetTime());

	//If CheckPoint Collision is true
	if (mCPColl)
	{
		mCPTimer += deltaTime;
	}
}

void HUD::Draw(Shader* shader)
{
	//Text
	DrawTexture(shader, mTimerText, Vector2(-420.0f, -325.0f));
	DrawTexture(shader, mCoinText, Vector2(-450.0f, -300.0f));
	CheckpointText(shader);

	//Radar
	DrawTexture(shader, mRadar, RADAR_POS);
	DrawTexture(shader, mRadarArrow, RADAR_POS);
	DrawCameras(shader);
}

std::string HUD::GetTime()
{
	float Time = mOwner->GetGame()->GetTime();
	int minutes = (int)(Time / 60.0f);
	int seconds = (int)fmod(Time, 60.0f);

	//Fraction
	int fraction;
	if (Time < 1.0f)
	{
		fraction = (int)(Time * 100);
	}
	else
	{
		fraction = (int)(100 * fmod(Time, seconds + minutes*60.0f));
	}
	//This
	if (seconds > 58 && minutes > 1)
	{
		fraction = fraction;
	}
	std::string min = std::to_string(minutes);
	std::string sec = std::to_string(seconds);
	std::string frac = std::to_string(fraction);
	min = min.substr(0, 2);
	sec = sec.substr(0, 2);
	frac = frac.substr(0, 2);
	if (min.length() == 1)
	{
		min = "0" + min;
	}
	if (sec.length() == 1)
	{
		sec = "0" + sec;
	}
	if (frac.length() == 1)
	{
		frac = "0" + frac;
	}
	return min + ":" + sec + "." + frac;
}

void HUD::AddCoin()
{
	mOwner->GetGame()->AddCoin();
	delete mCoinText;
	mCoinText = mFont->RenderText(std::to_string(mOwner->GetGame()->GetCoin()) + "/55");
}

void HUD::CheckpointText(Shader* shader)
{
	if (mCPColl)
	{
		if (mCPTimer <= CP_TIME)
		{
			delete mCPText;
			mCPText = mFont->RenderText(mCPDisplay);
			DrawTexture(shader, mCPText, Vector2::Zero);
		}
		else
		{
			mCPColl = false;
			mCPTimer = 0.0f;
		}
	}
}

void HUD::SetCPColl(bool coll)
{
	mCPColl = coll;
	mCPTimer = 0.0f;
}

void HUD::DrawCameras(Shader* shader)
{
	for (SecurityCamera* c : mOwner->GetGame()->GetCameras())
	{
		//Project to 2d plane
		Vector2 CamPos(c->GetPosition().x, c->GetPosition().y);
		Vector2 PlayerFor(mOwner->GetForward().x, mOwner->GetForward().y);
		Vector2 CamFor(c->GetWorldForward().x, c->GetWorldForward().y);
		Vector2 PlayPos(mOwner->GetPosition().x, mOwner->GetPosition().y);
		Vector2 ToCam = CamPos - PlayPos;

		//Find if within range of radar
		float DistToCam = ToCam.Length();
		if (DistToCam < RADAR_RANGE)
		{
			//Find Angle of position
			ToCam.Normalize();
			float PosRot = Math::Acos(Vector2::Dot(ToCam, PlayerFor));
			Vector3 a(ToCam.x, ToCam.y, 0.0f);
			Vector3 AxisRot = Vector3::Cross(mOwner->GetForward(), a);
			Matrix3 RotMat;
			if (AxisRot.z > 0)
			{
				RotMat = Matrix3::CreateRotation(-PosRot);
			}
			else
			{
				RotMat = Matrix3::CreateRotation(PosRot);
			}

			//Find the camera's rotation
			CamFor.Normalize();
			PlayerFor.Normalize();
			float CamRot = Math::Acos(Vector2::Dot(CamFor, PlayerFor));
			AxisRot = Vector3::Cross(mOwner->GetForward(), Vector3(CamFor.x, CamFor.y, 0.0f));
			if (AxisRot.z > 0)
			{
				CamRot *= -1.0f;
			}
			Vector2 RadarScale = PLAYER_FORWARD * DistToCam * (RADAR_RAD / RADAR_RANGE);
			Vector2 BlipPos = Vector2::Transform(RadarScale, RotMat);
			Vector2 ScreenPos = RADAR_POS + BlipPos;
			DrawTexture(shader, mBlip, ScreenPos, 1.0f, CamRot + Math::Pi/2);
		}
	}
}
