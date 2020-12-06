#pragma once

// TODO
#include "SDL/SDL.h"
#include "Component.h"
#include <vector>
#include <unordered_map>
#include "Math.h"

class Game
{
private:
	//Main Actors 
	class Player* Mario = nullptr;
	std::vector<class Actor*> Actors;
	std::vector<class Block*> mBlocks;
	std::vector<class Goomba*> mGoombas;

	//Window
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	Vector2 mCamera;

	//Map parameters
	const int WINDOW_HEIGHT = 448;
	const int WINDOW_WIDTH = 600;

	//Time Parameters
	float frameLimit = 16; //In miliseconds
	float lastFrame, deltaTime, currentTime;
	float maxdt = .033f;

	//Quit paramters
	bool DontQuit = true;

	//Data
	void LoadData();
	void UnloadData();
	int soundtrack;

	//Maps
	std::unordered_map<std::string, SDL_Texture*> TextureMap;
	std::unordered_map<std::string, struct Mix_Chunk*> SoundsMap;

	//Sprites
	std::vector<class SpriteComponent*> mSprites;

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	
	//Sound
	Mix_Chunk* GetSound(const std::string& filename);
	int GetMusicChannel();
	
	//Actors
	class Player* GetMario();
	void AddActor(class Actor* newActor);
	void RemoveActor(class Actor* oldActor);
	const std::vector<class Block*>& GetBlocks() const;  
	void AddBlock(class Block* newBlock);
	void RemoveBlock(class Block* oldBlock);
	const std::vector<class Goomba*>& GetGoombas() const;
	void AddGoomba(class Goomba* newGoomba);
	void RemoveGoomba(class Goomba* oldGoomba);
	
	//Sprites
	void AddSprite(class SpriteComponent* newSprite);
	void RemoveSprite(class SpriteComponent* oldSprite);
	SDL_Texture* GetTexture(std::string filename);

	//Camera 
	Vector2 GetCamera();
	void SetCamera(Vector2 dCamera);
};
