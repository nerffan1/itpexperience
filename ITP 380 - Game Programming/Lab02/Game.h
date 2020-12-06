#pragma once

// TODO
#include "SDL/SDL.h"
#include "Component.h"
#include <vector>
#include <unordered_map>

class Game
{
private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	//Vectors 
	std::vector<class Actor*> Actors;
	std::vector<class Asteroid*> Asteroids;

	//Map parameters
	const int WINDOW_HEIGHT = 768;
	const int WINDOW_WIDTH = 1024;

	//Time Parameters
	float frameLimit = 16; //In miliseconds
	float lastFrame, deltaTime, currentTime;
	float maxdt = 0.033f;

	//Quit paramters
	bool DontQuit = true;

	//Direction Parameters
	enum direction { nomov, up, down };
	direction movement = nomov;

	//Data
	void LoadData();
	void UnloadData();

	//Map
	std::unordered_map<std::string, SDL_Texture*> TextureMap;

	//Sprites
	std::vector<class SpriteComponent*> mSprites;

	class Ship* myShip = nullptr;

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
	void AddAsteroid(class Asteroid* newAsteroid);
	void RemoveAsteroid(class Asteroid* oldAsteroid);
	std::vector<Asteroid*> GetAsteroid();
	//Sprites
	void AddSprite(class SpriteComponent* newSprite);
	void RemoveSprite(class SpriteComponent* oldSprite);
	SDL_Texture* GetTexture(std::string filename);
};
