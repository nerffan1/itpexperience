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
#include "HUD.h"
#include "Player.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "LevelLoader.h"
#include "Checkpoint.h"
#include "Arrow.h"
#include "SDL/SDL_ttf.h"

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

	//Audio Support
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(32);
	Mix_GroupChannels(22, 31, 1);

	//Mouse Support
	SDL_SetRelativeMouseMode(SDL_TRUE); // Enable relative mouse mode
	SDL_GetRelativeMouseState(nullptr, nullptr); // Clear any saved values

	TTF_Init();
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
		LoadNextLevel();
	}
}

Player* Game::GetPlayer()
{
	return mPlayer;
}

void Game::RemoveCP()
{
	if (!mCheckpoints.empty())
	{
		mCheckpoints.pop();
	}
}

Checkpoint* Game::GetCheckpoint()
{
	if (mCheckpoints.empty())
	{
		return nullptr;
	}
	else
	{
		return mCheckpoints.front();
	}
}

void Game::LoadNextLevel()
{
	if (mTheHUD->GetCPTimer() == 0.0f)
	{
		if (!mNextLevel.empty())
		{
			std::vector<Actor*> destroyActors;
			for (Actor* actor : mActors)
			{
				actor->SetState(ActorState::Destroy);
			}
			for (auto actor : mActors)
			{
				if (actor->GetState() == ActorState::Destroy)
				{
					destroyActors.emplace_back(actor);
				}
			}
			for (auto actor : destroyActors)
			{
				delete actor;
			}
			LevelLoader::Load(this, mNextLevel);
			NewArrow();
			mTheHUD = new HUD(mPlayer);
			mNextLevel.clear();
		}
	}
}

HUD* Game::GetHUD()
{
	return mTheHUD;
}

void Game::NewArrow()
{
	mTheArrow = new Arrow(this);
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


}

void Game::GenerateOutput()
{
	// TODO: tell renderer to draw
	mRenderer->Draw();
}

void Game::LoadData()
{
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

	///Load Level
	LevelLoader::Load(this, "Assets/Tutorial.json");
	NewArrow();
	mTheHUD = new HUD(mPlayer);
	//SoundTrack = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
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

void Game::RemoveCoin(Coin* byeCoin)
{
	auto iter = std::find(mCoins.begin(), mCoins.end(), byeCoin);
	if (iter != mCoins.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mCoins.end() - 1;
		std::iter_swap(iter, iter2);
		mCoins.pop_back();
	}
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

void Game::AddBlock(Actor* actor)
{
	mBlocks.emplace_back(actor);
}

void Game::RemoveBlock(Actor* actor)
{
	auto iter = std::find(mBlocks.begin(), mBlocks.end(), actor);
	if (iter != mBlocks.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mBlocks.end() - 1;
		std::iter_swap(iter, iter2);
		mBlocks.pop_back();
	}
}

const std::vector<Actor*>& Game::GetBlocks() const
{
	return mBlocks;
}

void Game::AddCamera(SecurityCamera* actor)
{
	mCameras.push_back(actor);
}

void Game::RemoveCamera(SecurityCamera* actor)
{
	auto iter = std::find(mCameras.begin(), mCameras.end(), actor);
	if (iter != mCameras.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mCameras.end() - 1;
		std::iter_swap(iter, iter2);
		mCameras.pop_back();
	}
}
