//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

bool Game::Initialize()
{
	//Initialize audio and video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Create Window
	window = SDL_CreateWindow(
		"Pong",                  // window title
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

	// Declare first LastFrame
	lastFrame = (float)SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (DontQuit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
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
	else if (keyboard[SDL_SCANCODE_UP] && !keyboard[SDL_SCANCODE_DOWN])
	{
		movement = up;
	}
	else if (keyboard[SDL_SCANCODE_DOWN] && !keyboard[SDL_SCANCODE_UP])
	{
		movement = down;
	}
}

void Game::UpdateGame()
{
	//Frame Limiting: This won't let the difference of time between frames to be any less than 0.016 s
	while ((SDL_GetTicks() - lastFrame) < frameLimit)
	{
		//Do nothing
	}
	deltaTime = (float)(SDL_GetTicks() - lastFrame)/1000;
	if (deltaTime > maxdt)
	{
		deltaTime = maxdt;
	}

	//Adjust Paddle 
	switch (movement)
	{
	case up:
		if (PaddlePoint.y > WALL_THICKNESS)
		{
			PaddlePoint.y = PaddlePoint.y - (int)(deltaTime * 700);
		}
		break;
	case down:
		if (PaddlePoint.y < WINDOW_HEIGHT - (PADDLE_HEIGT + WALL_THICKNESS))
		{
			PaddlePoint.y = PaddlePoint.y + (int)(deltaTime * 700);
		}
		break;
	}
	movement = nomov;

	//Useful Variables
	int PADDLE_HITBOX[2] = {PaddlePoint.y - BALL_RADIUS,PaddlePoint.y + PADDLE_HEIGT + BALL_RADIUS};

	//Update X-axis and Y-axis
	deltaX += ((BallVelocity.x * deltaTime));
	deltaY += ((BallVelocity.y * deltaTime));
	if ((unsigned int)deltaX > 0 || ((unsigned int)deltaY > 0))
	{
		BallPoint.x += (int)deltaX;
		BallPoint.y += (int)deltaY;
		deltaX = 0; deltaY = 0;
	}

	//Adjust X-axis Ball boundaries
	if ((BallPoint.x + BALL_RADIUS) > RIGHT_LIMIT)
	{
		BallPoint.x = RIGHT_LIMIT - BALL_RADIUS;
	}
	else if (BallPoint.x < PADDLE_BOUND + 1 && BallPoint.y >= PADDLE_HITBOX[0] && BallPoint.y <= PADDLE_HITBOX[1])
	{
		BallPoint.x = PADDLE_BOUND;
	}

	//Adjust Y-axis Ball boundaries 
	if ((BallPoint.y + BALL_RADIUS) > BOTTOM_LIMIT)
	{
		BallPoint.y = BOTTOM_LIMIT - BALL_RADIUS;
	}
	else if (BallPoint.y < TOP_LIMIT)
	{
		BallPoint.y = TOP_LIMIT;
	}
	 
	//Check Boundaries to reflect off 
	bool PaddleBoundary = (BallPoint.y >= PADDLE_HITBOX[0] && BallPoint.y <= PADDLE_HITBOX[1] && BallPoint.x == PADDLE_BOUND);
	if ((BallPoint.x + BALL_RADIUS) == RIGHT_LIMIT)
	{
		BallVelocity.x *= -1;
	}
	else if (PaddleBoundary) 
	{
		BallVelocity.x *= -1;
		BallVelocity.x += DifficultyIncrease; //Velocity of x has to always be positive, so I can just add the difficulty increase
		(BallVelocity.y > 0) ? BallVelocity.y += DifficultyIncrease : BallVelocity.y -= DifficultyIncrease; //But, for Y-axis I must check the direction of increased difficulty
	}
	if ((BallPoint.y + BALL_RADIUS) == BOTTOM_LIMIT || BallPoint.y == TOP_LIMIT)
	{
		BallVelocity.y *= -1;
	}

	//Check if player loses
	if (BallPoint.x <= LOSE_ZONE)
	{
		DontQuit = false;
	}
	
	//Update frame 
	lastFrame = (float)SDL_GetTicks();
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	//Add Walls to the screen and Paddle
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect bottom, right, top, paddle, ball;

	//Define the paddle, wall, and ball paramters
	top.x = 0; top.y = 0; top.w = WINDOW_WIDTH; top.h = WALL_THICKNESS;
	bottom.x = 0; bottom.y = (WINDOW_HEIGHT - WALL_THICKNESS); bottom.w = WINDOW_WIDTH; bottom.h = WALL_THICKNESS;
	right.x = (WINDOW_WIDTH-WALL_THICKNESS); right.y = 0; right.w = WALL_THICKNESS; right.h = WINDOW_HEIGHT;
	paddle.x = PaddlePoint.x; paddle.y = PaddlePoint.y; paddle.w = PADDLE_WIDTH; paddle.h = PADDLE_HEIGT;
	ball.x = BallPoint.x; ball.y = BallPoint.y; ball.w = BALL_RADIUS; ball.h = BALL_RADIUS;
	SDL_RenderFillRect(renderer, &top);
	SDL_RenderFillRect(renderer, &right);
	SDL_RenderFillRect(renderer, &bottom);
	SDL_RenderFillRect(renderer, &paddle);
	SDL_RenderFillRect(renderer, &ball);
	SDL_RenderPresent(renderer);
}
