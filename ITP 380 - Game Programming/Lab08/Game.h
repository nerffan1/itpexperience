#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }

	//Map Update 
	void UpdateTunnel();

	//Player
	class Player* GetPlayer();
	int GetMusicChannel() { return engine; }

	//Additional Functions
	class HeightMap* GetHeightMap() { return mHM; }

	//Enemy 
	class CollisionComponent* GetEnemyColl();
	class Enemy* GetEnemy() { return mRrival; }

	//Time
	float mIntroBuffer = 8.5f;
	
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;
	class Player* mPlayer;
	class Enemy* mRrival;

	//Blocks
	std::vector<class SideBlock*> mSideBlocks;
	std::vector<std::vector<class Block*>> mBlockLayers;

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;

	//Helper Variable
	int engine;

	//Height Map 
	class HeightMap* mHM = nullptr;

	//Enemy Info
	class CollisionComponent* mEnemyColl = nullptr;

};
