//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.h"
#include "SideBlock.h"
#include "Block.h"

Game::Game()
	:mIsRunning(true)
{

}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		return false;
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::UpdateTunnel()
{
	//Spawn new Objects in Map
	BlockSpawner();
	ObstacleSpawner(1);
}

void Game::BlockSpawner()
{
	//Check that the 3000 mark has a block
	float ThreeMark = mPlayer->GetPosition().x + 2500.0f;
	SideBlock* lastBlok = mSideBlocks.back();
	float lastblockpos = lastBlok->GetPosition().x;
	if (lastblockpos < ThreeMark)
	{
		int color = Random::GetIntRange(1, 5);
		//Create a blok
		SideBlock* newBlok = new SideBlock(this, color);
		Vector3 newPos = lastBlok->GetPosition();
		newPos.x += 500.0f;
		newBlok->SetPosition(newPos);

		//We also need to create a blok opposite the new one
		SideBlock* newOther = new SideBlock(this, color);
		newPos.y *= -1; //The x coordinate is already set up
		newOther->SetPosition(newPos);
	}
}

void Game::ObstacleSpawner(int index)
{
	if (index == 0)
	{
		CreateBlockLayer(1, "", 0.0f); //LastBlockPos doesn't matter when string is empty
		FirstTwenty++;
	}
	else
	{
		float SpawnMark = mPlayer->GetPosition().x + 2000.0f;
		Block* lastBlok = mBlockLayers.back().back();
		float lastblockpos = lastBlok->GetPosition().x;
		if (lastblockpos < SpawnMark)
		{
			if (FirstTwenty <= 20)
			{
				CreateBlockLayer(2, std::to_string(FirstTwenty), lastblockpos);
				FirstTwenty++;
			}
			else
			{
				CreateBlockLayer(2, "random", lastblockpos);
			}
		}
	}
}

void Game::CreateBlockLayer(int init, std::string filen, float last)
{
	//Create an array of blocks for each layer
	std::vector<Block*> layer;
	mBlockLayers.push_back(layer);
	//Iterate and set cubes to proper positions
	for (int x = init; x <= 2; x++) //2 is arbitrary. It helps to just initiate the first 2 layers easily
	{
		//Set-Up parameters
		std::ifstream input;
		std::string filename = "Assets/Blocks/";
		if (filen.empty())
		{
			filen = std::to_string(x);
			last = (float)x * 1000.0f;
		}
		else if (filen == "random")
		{
			int a = Random::GetIntRange(1, 20);
			filen = std::to_string(a);
			last += 1000.0f;
		}
		else
		{
			last += 1000.0f;
		}
		filename += filen + ".txt";
		input.open(filename);

		//Loop through the .txt file
		for (int i = 0; i < 20; i++)
		{
			std::string line;
			std::getline(input, line);
			for (int j = 0; j < 20; j++)
			{
				if (line[j] != '.')
				{
					Vector3 offset(0.0f, -237.5f, 237.5f);
					Vector3 block(last, (float)j * 25.0f, (float)i * -25.0f);
					Vector3 pos = block + offset;
					Block* NewBlok = new Block(this, pos, line[j], LayerBuilt);
				}
			}
		}
		input.close();
	}
	LayerBuilt++;
}

void Game::AddSideBlok(SideBlock* blok)
{
	mSideBlocks.push_back(blok);
}

void Game::RemoveSideBlok(SideBlock* blok)
{
	mSideBlocks.erase(std::find(mSideBlocks.begin(), mSideBlocks.end(), blok));
}

void Game::AddBlok(Block* blok)
{
	mBlockLayers[LayerBuilt].push_back(blok);
}

void Game::RemoveBlok(Block* blok)
{
	std::vector<Block*> layer = mBlockLayers[blok->mLayer];
	layer.erase(std::find(layer.begin(), layer.end(), blok));
	if (layer.empty()) //If Layer becomes empty, delete it
	{
		auto beg = mBlockLayers.begin();
		auto end = mBlockLayers.end();
		mBlockLayers.erase(std::find(beg, end, mBlockLayers[blok->mLayer]));
		LayerBuilt = mBlockLayers.size();

		//Change all mLayer components 
		size_t layeri = 0;
		for (std::vector<Block*> layer : mBlockLayers)
		{
			for (Block* blok : layer)
			{
				blok->mLayer = layeri;
			}
			layeri++;
		}
	}
	else
	{
		mBlockLayers[blok->mLayer] = layer;
	}
}

const std::vector<std::vector<class Block*>>& Game::GetBlockLayers() const
{
	return mBlockLayers;
}

Player* Game::GetPlayer()
{
	return mPlayer;
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	std::vector<Actor*> currentActors = mActors; //Game would crash at the beginning because of this
	for (auto actor : currentActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();

	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}

	//Update Map
	UpdateTunnel();
}

void Game::GenerateOutput()
{
	// TODO: tell renderer to draw
	mRenderer->Draw();
}

void Game::LoadData()
{
	//Initilalize Player
	mPlayer = new Player(this);

	//Initialize Projection
	Matrix4 ProjMatrix = Matrix4::CreatePerspectiveFOV(
		1.22f,
		1024.0f,
		768.0f,
		10.0f,
		10000.0f
	);

	//Set View Matrix
	mRenderer->SetProjectionMatrix(ProjMatrix);
	Matrix4 ViewMatrix = Matrix4::CreateLookAt(
		Vector3(-300, 0, 100),
		Vector3(20, 0, 0),
		Vector3::UnitZ
	);
	mRenderer->SetViewMatrix(ViewMatrix);

	//Initiate the tunnel

	for (int i = 0; i < 6; i++)
	{
		int color = Random::GetIntRange(1, 5);
		SideBlock* RightBlok = new SideBlock(this, color);
		RightBlok->SetPosition(Vector3(500.0f * i, 500.0f, 0.0f));
		SideBlock* LeftBlok = new SideBlock(this, color);
		LeftBlok->SetPosition(Vector3(500.0f * i, -500.0f, 0.0f));
	}

	ObstacleSpawner(0);

	//Initiate Looping audio
	engine = Mix_PlayChannel(-1, GetSound("Assets/Sounds/ShipLoop.wav"), -1);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	// TODO: Delete renderer
	mRenderer->Shutdown();
	delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}
