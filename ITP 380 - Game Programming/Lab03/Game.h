#pragma once

// TODO
#include "SDL/SDL.h"
#include "Component.h"
#include <vector>
#include <unordered_map>

class Game
{
private:
	//Main Actors 
	std::vector<class Actor*> Actors;
	class Paddle* myPad = nullptr;
	std::vector<class Block*> mBlocks;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	//Map parameters
	const int WINDOW_HEIGHT = 768;
	const int WINDOW_WIDTH = 1024;

	//Time Parameters
	float frameLimit = 16; //In miliseconds
	float lastFrame, deltaTime, currentTime;
	float maxdt = .033f;

	//Quit paramters
	bool DontQuit = true;

	//Data
	void LoadData();
	void UnloadData();

	//Map
	std::unordered_map<std::string, SDL_Texture*> TextureMap;

	//Sprites
	std::vector<class SpriteComponent*> mSprites;

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	
	//Actors
	void AddActor(class Actor* newActor);
	void RemoveActor(class Actor* oldActor);
	class Paddle* GetPaddle();
	const std::vector<class Block*>& GetBlocks() const;
	void AddBlock(class Block* newBlock);
	void RemoveBlock(class Block* oldBlock);
	
	//Sprites
	void AddSprite(class SpriteComponent* newSprite);
	void RemoveSprite(class SpriteComponent* oldSprite);
	SDL_Texture* GetTexture(std::string filename);
};
