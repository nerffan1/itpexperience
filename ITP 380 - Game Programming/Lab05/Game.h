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
	//Actors 
	class Player* mPlayer = nullptr;
	std::vector<class Actor*> Actors;
	std::vector<class Collider*> mColliders;
	std::vector<std::string> mSpawnedRooms;

	//Window
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	Vector2 mCamera;

	//Map parameters
	const int WINDOW_HEIGHT = 448;
	const int WINDOW_WIDTH = 600;
	std::string mCurrentRoom = "Room1";

	//Time Parameters
	float frameLimit = 16; //In miliseconds
	float lastFrame, deltaTime, currentTime;
	float maxdt = .033f;
	int Soundtrack;

	//Quit paramters
	bool DontQuit = true;

	//Data
	void LoadData();
	void UnloadData();
	int soundtrack;

	//Maps
	std::unordered_map<std::string, SDL_Texture*> TextureMap;
	std::unordered_map<std::string, struct Mix_Chunk*> SoundsMap;
	std::unordered_map<std::string, std::vector<class Door*>> mDoorsMap;
	std::unordered_map<std::string, std::vector<class SecretBlock*>> mSBlocksMap;
	std::unordered_map<std::string, std::vector<class Spawner*>> mSpawnersMap;
	std::unordered_map<std::string, std::vector<class Actor*>> mEnemiesMap;

	//Sprites
	std::vector<class SpriteComponent*> mSprites;

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	
	//Map Tools
	void LoadRoom(std::string RoomFile, std::string RoomName);
	void SetNewRoom(std::string room);
	std::string GetCurrentRoom();
	void SpawnNewRoomActors(); // Use this function to spawn objects in room;

	//Sound
	Mix_Chunk* GetSound(const std::string& filename);
	int GetMusicChannel();
	
	//Actors
	class Player* GetPlayer();
	void AddActor(class Actor* newActor);
	void RemoveActor(class Actor* oldActor);
	//Doors
	void AddDoor(Door* newDoor, std::string RoomName);
	const std::vector<Door*>& Game::GetDoors();
	//Colliders
	void AddCollider(class Collider* newCollider);
	const std::vector<Collider*>& GetColliders() const;
	//SecretBlocks
	void AddSecretBlock(class SecretBlock* newBlock, std::string RoomName);
	const std::vector<SecretBlock*>& GetSBlocks();
	//Spawners
	void AddSpawner(class Spawner* newSpawn, std::string RoomName);
	const std::vector<Spawner*>& GetSpawners();
	void RemoveSpawner(class Spawner* ByeSpawn);

	//Enemy Component 
	void AddEnemy(class Actor* newEnemy, std::string RoomName);
	const std::vector<class Actor*>& GetEnemies();
	void RemoveEnemy(class Actor* ByeActor);
	
	//Sprites
	void AddSprite(class SpriteComponent* newSprite);
	void RemoveSprite(class SpriteComponent* oldSprite);
	SDL_Texture* GetTexture(std::string filename);

	//Camera 
	Vector2 GetCamera();
	void SetCamera(Vector2 dCamera);
	void PanCamera();
};
