//#include <SDL.h>
#include <stdio.h>

#include "Enginator5000.h"

////	Declare SDL main. You can't change this form. It should be int main(int, char**)
int main(int argc, char* args[])
{
	// Check whether SDL is ready
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		printf("Failed at initialization! SDL_ERROR: %s\n", SDL_GetError());
	}
	else
	{
		Enginator5000 engine;
		engine.loadScene("Scenes\\CubeScene\\cubeGame.xml");

		//EngineWindow engine;
		////engine.run();
		//engine.runLineExample();
	}



	SDL_Quit();

	return 0;
}