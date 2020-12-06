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
	void BlockSpawner();
	void ObstacleSpawner(int index); //If 0, do startup blocks; else do rest of the blocks 
	void CreateBlockLayer(int init,std::string filename, float last);

	//SideBlocks
	void AddSideBlok(class SideBlock* blok);
	void RemoveSideBlok(class SideBlock* blok);

	//Blocks
	void AddBlok(class Block* blok);
	void RemoveBlok(class Block* blok);
	const std::vector<std::vector<class Block*>>& GetBlockLayers() const;

	//Player
	class Player* GetPlayer();
	int GetMusicChannel() { return engine; }

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

	//Blocks
	std::vector<class SideBlock*> mSideBlocks;
	std::vector<std::vector<class Block*>> mBlockLayers;

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;

	//Constant
	const float DeleteCoor = 450.0f;

	//Helper Variable
	int FirstTwenty = 1;
	int LayerBuilt = 0;
	int engine;
};
