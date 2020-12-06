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
#include "Block.h"
#include "Player.h"
#include <fstream>
#include <string>
#include "Spawner.h"
#include "SDL/SDL_mixer.h"

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
		"Mario",                  // window title
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
	lastFrame = (float) SDL_GetTicks();

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
	lastFrame = (float) SDL_GetTicks(); 
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(renderer,0, 0, 255, 255);
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
			SDL_Log("Texture %s failed failed to load",filename.c_str());
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

Player* Game::GetMario()
{
	return Mario;
}

void Game::AddActor(Actor* newActor)
{
	Actors.push_back(newActor);
}

void Game::RemoveActor(Actor* oldActor)
{
	Actors.erase(std::find(Actors.begin(), Actors.end(), oldActor));
}

const std::vector<class Block*>& Game::GetBlocks() const 
{
	return mBlocks;
}

void Game::AddBlock(Block* newBlock)
{
	mBlocks.push_back(newBlock);
}

void Game::RemoveBlock(Block* oldBlock)
{
	mBlocks.erase(std::find(mBlocks.begin(), mBlocks.end(), oldBlock));
}

const std::vector<class Goomba*>& Game::GetGoombas() const
{
	return mGoombas;
}

void Game::AddGoomba(Goomba* newGoomba)
{
	mGoombas.push_back(newGoomba);
}

void Game::RemoveGoomba(Goomba* oldGoomba)
{
	mGoombas.erase(std::find(mGoombas.begin(), mGoombas.end(), oldGoomba));
}

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
			SDL_Log("Texture failed failed to load");
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

Vector2 Game::GetCamera()
{
	return mCamera;
}

void Game::SetCamera(Vector2 dCamera)
{
	mCamera = dCamera;
}

void Game::LoadData()
{
	//Initialize the Camera
	mCamera = Vector2::Zero;

	//Background
	Actor* background = new Actor(this);
	SpriteComponent* back = new SpriteComponent(background, 1);
	back->SetTexture(GetTexture("Assets/Background.png"));
	back->SetIsVisible(true);
	background->SetScale(1);
	Vector2 backg(3392, 224);
	background->SetPosition(backg);

	//Draw the blocks according Level.txt
	std::ifstream input;
	input.open("Assets/Level1.txt");
	std::string line;
	std::getline(input, line);
	int columns = line.size();
	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			//Position Vector
			Vector2 pos(16.0f + (float)j * 32.0f, 16.0f + (float)i * 32.0f);
			if (line[j] != '.' && line[j] != 'P' && line[j] != 'Y')
			{
				Block* block = new Block(this, pos, line[j]);
			}
			else if (line[j] == 'P')
			{
				Mario = new Player(this);
				Mario->SetPosition(pos);
			}
			else if (line[j] == 'Y')
			{
				Spawner* Spawn = new Spawner(this,pos);
			}
		}
		std::getline(input, line);
	}
	//Load Music
	soundtrack = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
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
	for (std::pair<std::string,SDL_Texture*> i : TextureMap)
	{
		SDL_DestroyTexture(TextureMap[i.first]);
	}

	//Free sounds
	for (std::pair<std::string, Mix_Chunk*> i : SoundsMap)
	{
		Mix_FreeChunk(i.second);
	}
}
