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
	
	//Add Blocks
	void AddBlock(class Actor* actor);
	void RemoveBlock(class Actor* actor);
	const std::vector<class Actor*>& GetBlocks() const;

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }

	//Player
	class Player* GetPlayer();
	void SetPlayer(class Player* playr) { mPlayer = playr; }
	int GetMusicChannel() { return SoundTrack; }
	
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mBlocks;
	class Player* mPlayer = nullptr;

	//Blocks
	std::vector<class SideBlock*> mSideBlocks;
	std::vector<std::vector<class Block*>> mBlockLayers;

	class Renderer* mRenderer = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning;

	//Helper Variable
	int SoundTrack = 0 ;
};
