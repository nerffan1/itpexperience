#pragma once

// TODO
#include "SDL/SDL.h"
#include <sstream>
#include <iostream>
class Game
{
private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	//Map parameters
	const int WALL_THICKNESS = 20;
	const int WINDOW_HEIGHT = 768;
	const int WINDOW_WIDTH = 1024;
	const int TOP_LIMIT = WALL_THICKNESS;
	const int BOTTOM_LIMIT = WINDOW_HEIGHT - WALL_THICKNESS;
	const int RIGHT_LIMIT = WINDOW_WIDTH - WALL_THICKNESS;
	const int LOSE_ZONE = 0;

	//Time Parameters
	int frameLimit = 16; //In miliseconds
	float lastFrame, deltaTime, currentTime;
	float maxdt = 0.033f;

	//Paddle Parameters
	const int PADDLE_HEIGT = 200;
	const int PADDLE_WIDTH = WALL_THICKNESS / 2;
	const int PADDLE_BOUND = PaddlePoint.x + PADDLE_WIDTH;
	SDL_Point PaddlePoint = { WALL_THICKNESS / 2,WINDOW_HEIGHT / 2 - PADDLE_HEIGT / 2 };

	//Ball Parameters
	SDL_Point BallPoint = { WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT/2 - 10};
	const int BALL_RADIUS = 10;
	SDL_Point BallVelocity = {150,150};
	const int DifficultyIncrease = 50;
	float deltaY = 0, deltaX = 0;
	bool DontQuit = true;

	//Direction Parameters
	enum direction {nomov,up,down};
	direction movement = nomov;

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
};
