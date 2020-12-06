//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "SDL/SDL_image.h"
#include "MoveComponent.h"
#include <algorithm>
#include "Random.h"
#include "Player.h"
#include <string>
#include "Spawner.h"
#include "SDL/SDL_mixer.h"
#include "TiledBGComponent.h"
#include <fstream>
#include <sstream>
#include "CSVHelper.h"
#include "Collider.h"
#include "Door.h"
#include "SecretBlock.h"

// TODO

void Game::RunLoop()
{
	while (DontQuit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

bool Game::Initialize()
{
	//Initialize video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Create Window
	window = SDL_CreateWindow(
		"The Legend of Zelda",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WINDOW_WIDTH,                      // width, in pixels
		WINDOW_HEIGHT,                     // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);
	if (window == NULL) {
		SDL_Log("Unable to initialize window", SDL_GetError());
		return false;
	}

	//Create Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_Log("Unable to initialize renderer: %s", SDL_GetError());
		return false;
	}

	//Initilialize SDL image
	IMG_Init(IMG_INIT_PNG);

	//Initialize video
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Load Data
	LoadData();

	// Declare first LastFrame
	lastFrame = (float)SDL_GetTicks();

	return true;
}

void Game::ProcessInput()
{
	// Create event and check for particular events
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			DontQuit = false;
		}
	}
	// Keyboard Input
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	if (keyboard[SDL_SCANCODE_ESCAPE])
	{
		DontQuit = false;
	}
	std::vector<Actor*> ActorsCopy = Actors;
	for (Actor* ProcessActor : ActorsCopy)
	{
		ProcessActor->ProcessInput(keyboard);
	}
}

void Game::UpdateGame()
{
	//Time: Frame Limiting and Maximum
	while ((SDL_GetTicks() - lastFrame) < frameLimit)
	{
		//Do nothing
	}
	deltaTime = (float)(SDL_GetTicks() - lastFrame) / 1000;
	if (deltaTime > 0.033)
	{
		deltaTime = 0.033f;
	}

	// Update actors
	std::vector<class Actor*> ActorsCopy = Actors;
	for (Actor* i : ActorsCopy)
	{
		i->Update(deltaTime);
	}
	std::vector<Actor*> ActorsDestroy;
	for (Actor* goodbyeActor : ActorsCopy)
	{
		if (goodbyeActor->GetState() == ActorState::Destroy)
		{
			ActorsDestroy.push_back(goodbyeActor);
		}
	}
	for (Actor* goodbyeActor : ActorsDestroy)
	{
		delete goodbyeActor;
	}

	//Get Frame			
	lastFrame = (float)SDL_GetTicks();
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	//Draw sprites
	for (SpriteComponent* spriteElement : mSprites)
	{
		if (spriteElement->IsVisible())
		{
			spriteElement->Draw(renderer);
		}
	}

	SDL_RenderPresent(renderer);
}

Mix_Chunk* Game::GetSound(const std::string& filename)
{
	if (SoundsMap.find(filename) == SoundsMap.end())
	{
		//Add sound if it doesn't exist in map
		Mix_Chunk* newSound = Mix_LoadWAV(filename.c_str());
		if (newSound == nullptr)
		{
			SDL_Log("Texture %s failed failed to load", filename.c_str());
		}
		else
		{
			SoundsMap[filename] = newSound;
		}
	}
	return SoundsMap[filename];
}

int Game::GetMusicChannel()
{
	return soundtrack;
}

void Game::Shutdown()
{
	UnloadData();
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	SDL_Quit();
}

//Actors
void Game::AddActor(Actor* newActor)
{
	Actors.push_back(newActor);
}

void Game::RemoveActor(Actor* oldActor)
{
	Actors.erase(std::find(Actors.begin(), Actors.end(), oldActor));
}

//Player
Player* Game::GetPlayer()
{
	return mPlayer;
}

//Sprite
void Game::AddSprite(SpriteComponent* newSprite)
{
	mSprites.push_back(newSprite);
	//Sort the vector
	std::sort(mSprites.begin(), mSprites.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
			return a->GetDrawOrder() < b->GetDrawOrder();
		});
}

void Game::RemoveSprite(SpriteComponent* oldSprite)
{
	mSprites.erase(std::find(mSprites.begin(), mSprites.end(), oldSprite));
}

SDL_Texture* Game::GetTexture(std::string filename)
{
	if (TextureMap.find(filename) == TextureMap.end())
	{
		//Create texture if it doesn't exist in map
		SDL_Surface* newSurface = IMG_Load(filename.c_str());
		if (newSurface == nullptr)
		{
			SDL_Log("Texture %s failed to load", filename.c_str());
		}
		else
		{
			SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, newSurface);
			TextureMap[filename] = newTexture;
			SDL_FreeSurface(newSurface);
		}
	}
	return TextureMap[filename];
}

//Doors
void Game::AddDoor(Door* newDoor, std::string RoomName)
{
	mDoorsMap[RoomName].push_back(newDoor);
}
const std::vector<Door*>& Game::GetDoors()
{
	return mDoorsMap[mCurrentRoom];
}

//Collider
void Game::AddCollider(Collider* newCollider)
{
	mColliders.push_back(newCollider);
}

const std::vector<Collider*>& Game::GetColliders() const
{
	return mColliders;
}

//Secret Blocks
void Game::AddSecretBlock(SecretBlock* newBlock, std::string RoomName)
{
	mSBlocksMap[RoomName].push_back(newBlock);
}

const std::vector<SecretBlock*>& Game::GetSBlocks()
{
	return mSBlocksMap[mCurrentRoom];
}

//Spawners
void Game::AddSpawner(Spawner* newSpawn, std::string RoomName)
{
	mSpawnersMap[RoomName].push_back(newSpawn);
}

const std::vector<Spawner*>& Game::GetSpawners()
{
	return mSpawnersMap[mCurrentRoom];
}

void Game::RemoveSpawner(Spawner* ByeSpawn)
{
	std::vector<Spawner*> Spawns = mSpawnersMap[mCurrentRoom];
	Spawns.erase(std::find(Spawns.begin(), Spawns.end(), ByeSpawn));
	mSpawnersMap[mCurrentRoom] = Spawns;
}

//Enemy Component 
void Game::AddEnemy(Actor* newEnemy, std::string RoomName)
{
	mEnemiesMap[RoomName].push_back(newEnemy);
}

const std::vector<Actor*>& Game::GetEnemies()
{
	return mEnemiesMap[mCurrentRoom];
}

void Game::RemoveEnemy(Actor* ByeActor)
{
	std::vector<Actor*> ByeActors = mEnemiesMap[mCurrentRoom];
	ByeActors.erase(std::find(ByeActors.begin(), ByeActors.end(), ByeActor));
	mEnemiesMap[mCurrentRoom] = ByeActors;
}

//Camera
Vector2 Game::GetCamera()
{
	return mCamera;
}

void Game::SetCamera(Vector2 dCamera)
{
	mCamera = dCamera;
}

void Game::PanCamera()
{
}

//Load Data
void Game::LoadData()
{
	//Set Camera
	//Vector2 updateCamera((float)(15 * 32), (float)(20.5 * 32));
	//SetCamera(updateCamera);
	//Create Background
	Actor* Background = new Actor(this);
	TiledBGComponent* mappingComp = new TiledBGComponent(Background);

	//Load Rooms
	std::string Room;
	std::string RoomFile;
	for (int i = 1; i <= 7; i++)
	{
		Room = "Room" + std::to_string(i);
		RoomFile = "Assets/Dungeon/Room" + std::to_string(i) + ".csv";
		LoadRoom(RoomFile, Room);
	}

	//Load Music
	Soundtrack = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
}

void Game::UnloadData()
{
	//Delete remaining actors
	if (!Actors.empty())
	{
		for (size_t i = 0; i < Actors.size(); i++)
		{
			delete Actors.back();
		}
	}

	//Destroy Textures
	for (std::pair<std::string, SDL_Texture*> i : TextureMap)
	{
		SDL_DestroyTexture(TextureMap[i.first]);
	}

	//Free sounds
	for (std::pair<std::string, Mix_Chunk*> i : SoundsMap)
	{
		Mix_FreeChunk(i.second);
	}
}

//Room Dynamics
void Game::SetNewRoom(std::string room)
{
	//Pause the enemies in room that you're leaving
	for (Actor* enemy : mEnemiesMap[mCurrentRoom])
	{
		enemy->SetState(ActorState::Paused);
	}

	//Change to a new Room
	mCurrentRoom = room;
	SpawnNewRoomActors();
}

std::string Game::GetCurrentRoom()
{
	return mCurrentRoom;
}

void Game::SpawnNewRoomActors()
{
	//Find if room has been spawned before.
	std::vector<std::string>::iterator it = std::find(mSpawnedRooms.begin(), mSpawnedRooms.end(), mCurrentRoom);
	if (it == mSpawnedRooms.end())
	{
		//Spawn the items if room hasn't been visited, and add to list.
		for (Spawner* spawns : mSpawnersMap[mCurrentRoom])
		{
			spawns->SpawnObject();
		}
		mSpawnedRooms.push_back(mCurrentRoom);
	}
	else //If you've been there before, just reactivate old enemies
	{
		for (Actor* enemy : mEnemiesMap[mCurrentRoom])
		{
			enemy->SetState(ActorState::Active);
		}
	}
}

void Game::LoadRoom(std::string RoomFile, std::string RoomName)
{
	float coor[2];

	//Loop through the file and find the Player
	std::ifstream RoomToLoad;
	RoomToLoad.open(RoomFile);
	if (RoomToLoad.is_open())
	{
		std::vector<std::string> rows;
		std::string line;
		std::getline(RoomToLoad, line);
		std::getline(RoomToLoad, line); //Run a second time to jump over column names
		rows = CSVHelper::Split(line);
		while (!RoomToLoad.eof())
		{
			if (rows[0] == "Collider")
			{
				float width = (float)std::stoi(rows[3]);
				float height = (float)std::stoi(rows[4]);
				coor[0] = (float)std::stoi(rows[1]) + width / 2;
				coor[1] = (float)std::stoi(rows[2]) + height / 2;
				Collider* coll = new Collider(width, height, coor[0], coor[1], this);
			}
			else if (rows[0] == "Player")
			{
				coor[0] = (float)std::stoi(rows[1]);
				coor[1] = (float)std::stoi(rows[2]);
				mPlayer = new Player(this);
				Vector2 LinkStart(coor[0], coor[1]);
				mPlayer->SetPosition(LinkStart);
			}
			else if (rows[0] == "Door")
			{
				float width = (float)std::stoi(rows[3]);
				float height = (float)std::stoi(rows[4]);
				coor[0] = (float)std::stoi(rows[1]) + width / 2;
				coor[1] = (float)std::stoi(rows[2]) + height / 2;
				Vector2 DoorPos(coor[0], coor[1]);
				std::string params[3] = { rows[5],rows[6],rows[7] };
				Door* newDoor = new Door(DoorPos, RoomName, params, this);
			}
			else if (rows[0] == "SecretBlock")
			{
				float width = (float)std::stoi(rows[3]);
				float height = (float)std::stoi(rows[4]);
				coor[0] = (float)std::stoi(rows[1]) + width / 2;
				coor[1] = (float)std::stoi(rows[2]) + height / 2;
				Vector2 BlokPos(coor[0], coor[1]);
				SecretBlock* newSBlock = new SecretBlock(BlokPos, rows[6], RoomName, width, height, this);
			}
			else
			{
				float width = (float)std::stoi(rows[3]);
				float height = (float)std::stoi(rows[4]);
				coor[0] = (float)std::stoi(rows[1]) + width / 2;
				coor[1] = (float)std::stoi(rows[2]) + height / 2;
				Vector2 SpawnPos(coor[0], coor[1]);
				Spawner* newSpawn = new Spawner(this, SpawnPos, rows[0], RoomName);
			}
			std::getline(RoomToLoad, line);
			rows = CSVHelper::Split(line);
		}
	}
}