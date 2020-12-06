#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <queue>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	
	//Add Blocks
	void AddBlock(class Actor* actor);
	void RemoveBlock(class Actor* actor);
	const std::vector<class Actor*>& GetBlocks() const;

	//Coins
	void AddCoin(class Coin* newCoin) { mCoins.push_back(newCoin); }
	void RemoveCoin(class Coin* byeCoin);
	const std::vector<class Coin*>& GetCoins() const { return mCoins; }

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }

	//Player
	class Player* GetPlayer();
	void SetPlayer(class Player* playr) { mPlayer = playr; }
	int GetSoundTrack() { return SoundTrack; }

	//Checkpoints
	void AddCP(class Checkpoint* CP) { mCheckpoints.push(CP); }
	void RemoveCP();
	class Checkpoint* GetCheckpoint();
	void SetNextLevel(std::string level) { mNextLevel = level; }

	//Misc
	void LoadNextLevel();


private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::queue<class Checkpoint*> mCheckpoints;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mBlocks;
	class Player* mPlayer = nullptr;
	std::vector<class Coin*> mCoins;
	class Arrow* TheArrow = nullptr;

	//Blocks
	std::vector<class SideBlock*> mSideBlocks;
	std::vector<std::vector<class Block*>> mBlockLayers;

	class Renderer* mRenderer = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning;

	//Helper Variable
	int SoundTrack = 0 ;
	std::string mNextLevel = "";

};
