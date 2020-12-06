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
	Game Zelda;
	bool success = Zelda.Initialize();
	if (success)
	{
		Zelda.RunLoop();
	}
	Zelda.Shutdown();
	return 0;
}
