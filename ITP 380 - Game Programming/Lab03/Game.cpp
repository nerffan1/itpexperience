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
#include <fstream>
#include <string>
#include "Paddle.h"
#include "Ball.h"

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
	//Initialize audio and video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Create Window
	window = SDL_CreateWindow(
		"BreakThrough",                  // window title
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

	//Load Data
	LoadData();

	// Declare first LastFrame
	lastFrame = SDL_GetTicks();

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
		deltaTime = 0.033;
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
	lastFrame = SDL_GetTicks(); //DOES THIS GO HERE
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

void Game::Shutdown()
{
	UnloadData();
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::AddActor(Actor* newActor)
{
	Actors.push_back(newActor);
}

void Game::RemoveActor(Actor* oldActor)
{
	Actors.erase(std::find(Actors.begin(), Actors.end(), oldActor));
}

Paddle* Game::GetPaddle()
{
	return myPad;
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

void Game::LoadData()
{
	//Background
	Actor* background = new Actor(this);
	SpriteComponent* back = new SpriteComponent(background, 50);
	back->SetTexture(GetTexture("Assets/Background.png"));
	back->SetIsVisible(true);
	background->SetScale(1);
	Vector2 backg(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	background->SetPosition(backg);
	
	//Draw the blocks according Level.txt
	std::ifstream input;
	input.open("Assets/Level.txt");
	std::string line;
	for (int i = 0; i < 15; i++)
	{
		std::getline(input, line);
		for (int j = 0; j < 15; j++)
		{
			if (line[j] != '.')
			{
				Vector2 pos(64.0f + (float)j * 64.0f, 48.0f + (float)i * 32.0f);
				Block* block = new Block(this, pos, line[j]);
			}
		}
	}

	//Paddle
	myPad = new Paddle(this);
	Ball* myBall = new Ball(this);

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
}
