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
	Game SuperMario;
	bool success = SuperMario.Initialize();
	if (success)
	{
		SuperMario.RunLoop();
	}
	SuperMario.Shutdown();
	return 0;
}
