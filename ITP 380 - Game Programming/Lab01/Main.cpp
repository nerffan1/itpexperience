//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <SDL/SDL.h>

int main(int argc, char** argv)
{
	// TODO
	Game pong;
	bool success = pong.Initialize();
	if (success)
	{
		pong.RunLoop();
	}
	pong.Shutdown();
	return 0;
}
