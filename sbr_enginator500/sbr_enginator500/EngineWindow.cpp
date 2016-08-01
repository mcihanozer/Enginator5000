// INCLUDES
#include "EngineWindow.h"

#include<iostream>	// For using cout

// CONSTRUCTORS

#pragma warning( disable : 4996)	// For making Visual Studio shut up about string.copy()

// Create Enginator5000 window for display using the properties read from XML file
EngineWindow::EngineWindow(std::map<string, string>& propertyList, std::vector<FileList> sceneList)
{
	int width = 1024, height = 768, winPosX = SDL_WINDOWPOS_CENTERED, winPosY = SDL_WINDOWPOS_CENTERED;
	SDL_WindowFlags flags = SDL_WINDOW_SHOWN;


	if (propertyList.find("width") != propertyList.end() )	// width property is set
	{
		width = stoi(propertyList.find("width")->second);
	}

	if (propertyList.find("height") != propertyList.end())	// height property is set
	{
		height = stoi(propertyList.find("height")->second);
	}

	if (propertyList.find("position") != propertyList.end())	// position property is set
	{
		string pos = propertyList.find("position")->second;

		if (pos.compare("centered") != 0)	// Window position is not centered
		{
			float arr[2];
			strToVec2(pos, arr);

			winPosX = (int)arr[0];
			winPosY = (int)arr[1];
		}
	}

	if (propertyList.find("isFullScreen") != propertyList.end())	// isFullScreen property is set
	{
		if (propertyList.find("isFullScreen")->second.compare("true") == 0)
		{
			flags = (SDL_WindowFlags)(flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
	}

	setMembers("Enginator5000", winPosX, winPosY, width, height, flags);

	mSceneList = sceneList;

}

// Create Enginator5000 window for display: Creates	1024x768 centered windows
EngineWindow::EngineWindow(SDL_WindowFlags flags)
{
	// Set members and start the program
	setMembers("Enginator5000", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, flags);

	//setMembers("Enginator5000", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 1200, flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
}

// Create Enginator5000 window for display: Create centered window with given dimension
EngineWindow::EngineWindow(int width, int height, SDL_WindowFlags flags)
{
	// Set members and start the program
	setMembers("Enginator5000", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
}

// Create Enginator5000 window for display with given position and dimension
EngineWindow::EngineWindow(int winPosX, int winPosY, int width, int height, SDL_WindowFlags flags)
{
	// Set members and start the program
	setMembers("Enginator5000", winPosX, winPosY, width, height, flags);
}

// PUBLIC METHODS

// Initialize elements: Read and set next scene
void EngineWindow::init()
{
	mScene.loadScene(mSceneList[currentSceneIndex].path.c_str(), &mRasterizer);	// Update scene
}

// Start running: Takes events, refresh the buffers etc.
void EngineWindow::run()
{
	isRun = true;	// For keep running
	SDL_Event e;	// For getting events

	float magnifier = 50;

	unsigned int lastTicks = SDL_GetTicks();	// for FPS

	while (isRun)
	{
		// Update delta-time if necessary
		long now = SDL_GetTicks();

		if (now > mLast)	// Update delta-time
		{
			mDeltaTime = ( (float)(now - mLast) ) / 1000.f;
			mLast = now;
		}

		while (SDL_PollEvent(&e) != 0)
			handleEvents(e);

		// START DRAWING OPERATION	\\
		// 1. Lock  and clear the frame buffer
		// 2. Update the scene
		// 3. Draw
		// 4. Unlock the frame buffer and update

		// Step #1: Lock and the buffter
		SDL_LockSurface(mSurface);
		mRasterizer.clear();

		// Step #2: Update the scene
		mScene.updateScene();

		// Step #3: Draw
		/*Color red(1, 0, 0);

		for (int i = 0; i < mScene.mMeshList[0]->mFace.size(); i++)
		{
			Face currentFace = mScene.mMeshList[0]->mFace[i];

			Vertex v1 = mScene.mMeshList[0]->mVert[currentFace.mIndex[0]];
			Vertex v2 = mScene.mMeshList[0]->mVert[currentFace.mIndex[1]];
			Vertex v3 = mScene.mMeshList[0]->mVert[currentFace.mIndex[2]];

			mRasterizer.drawLine(v1.mPos.x * magnifier, v1.mPos.y * magnifier, red, v2.mPos.x * magnifier, v2.mPos.y * magnifier, red);
			mRasterizer.drawLine(v1.mPos.x * magnifier, v1.mPos.y * magnifier, red, v3.mPos.x * magnifier, v3.mPos.y * magnifier, red);
			mRasterizer.drawLine(v2.mPos.x * magnifier, v2.mPos.y * magnifier, red, v3.mPos.x * magnifier, v3.mPos.y * magnifier, red);
		}*/

		// Step #4: Unlock the frame buffer and update
		SDL_UnlockSurface(mSurface);
		SDL_UpdateWindowSurface(mWindow);

		// calculate the number of seconds that

		// have passed since the last update
		unsigned int ticks = SDL_GetTicks();
		unsigned int ticksDiff = ticks - lastTicks;
		if (ticksDiff == 0)
			continue;
		float time = ticksDiff / 1000.0f;
		lastTicks = ticks;

		// display frames per second
		unsigned int fps = 1000 / ticksDiff;

		
		//if (fps < 60)
		//{
		//	int a = -9;
		//	printf("Frames per second: %u    \n", fps);
		//	scanf("%d", a);
		//}

		//printf("Frames per second: %u    \n", fps);


	}	// End of while(isRun)

	clear();

}	// End of run()

// Run line drawing example
void EngineWindow::runLineExample()
{
	float r = 0.0f;
	unsigned int lastTicks = SDL_GetTicks();

	isRun = true;	// For keep running
	SDL_Event e;	// For getting events

	int winWidth, winHeight;
	SDL_GetWindowSize(mWindow, &winWidth, &winHeight);

	while (isRun)
	{
		while (SDL_PollEvent(&e) != 0)
			handleEvents(e);

		// START DRAWING OPERATION	\\
		// 1. Lock the frame buffer
		// 2. Make calculations
		// 3. Draw
		// 4. Unlock the frame buffer and update

		// Step 1: Lock the buffer
		SDL_LockSurface(mSurface);
		mRasterizer.clear();

		// Step 2: Make calcualtions

		// calculate coordinates for triangle
		const float size = 110.f;
		float x1 = (winWidth / 2) + cosf(r - M_PI / 6.0) * size;
		float y1 = (winHeight / 2) + sinf(r - M_PI / 6.0) * size;
		float x2 = (winWidth / 2) + cosf(r + M_PI / 2.0) * size;
		float y2 = (winHeight / 2) + sinf(r + M_PI / 2.0) * size;
		float x3 = (winWidth / 2) + cosf(r + M_PI + M_PI / 6.0) * size;
		float y3 = (winHeight / 2) + sinf(r + M_PI + M_PI / 6.0) * size;

		// colors for each point of the triangle
		Color color1(1.0f, 0.0f, 0.0f);
		Color color2(0.0f, 1.0f, 0.0f);
		Color color3(0.0f, 0.0f, 1.0f);
		Color color4(1.f, 0.f, 1.f);
		Color color5(1.f, 1.f, 0.f);
		Color color6(0.f, 1.f, 1.f);

		//Color color1(1.0f, 0.0f, 0.0f);
		//Color color2(0.0f, 1.0f, 0.0f);
		//Color color3(0.0f, 0.0f, 1.0f);

		// Step 3: Draw
		//------------------------	ROTATION TRIANGLE	------------------------//

		//mRasterizer.drawLine(x1, y1, color1, x2, y2, color2);
		//mRasterizer.drawLine(x2, y2, color2, x3, y3, color3);
		//mRasterizer.drawLine(x3, y3, color3, x1, y1, color1);

		mRasterizer.rasterTriangle(x1, y1, color1, x2, y2, color2, x3, y3, color3);

		//------------------------	SIMPLE TRIANGLE	------------------------//

		//mRasterizer.drawLine(800, 100, color1, 800, 600, color2);
		//mRasterizer.drawLine(800, 600, color2, 1200, 600, color3);
		//mRasterizer.drawLine(1200, 600, color3, 800, 100, color1);

		//------------------------	TEST	------------------------//

		mRasterizer.drawLine(800, 0, color1, 800, 1199, color1);	// |
		mRasterizer.drawLine(0, 600, color2, 1599, 600, color2);	// --
		mRasterizer.drawLine(0, 0, color3, 1599, 1199, color3);	// \ //
		mRasterizer.drawLine(1599, 0, color4, 0, 1199, color4);	// /

		mRasterizer.drawLine(0, 300, Color(), 1599, 899, Color());

		mRasterizer.drawLine(300, 0, color5, 1299, 1199, color5);

		mRasterizer.drawLine(1259, 0, color6, 260, 1299, color6);

		// Step 4: Unlock and update
		SDL_UnlockSurface(mSurface);
		SDL_UpdateWindowSurface(mWindow);

		// calculate the number of seconds that
		unsigned int ticks = SDL_GetTicks();
		unsigned int ticksDiff = ticks - lastTicks;
		if (ticksDiff == 0)
			continue;
		float time = ticksDiff / 1000.0f;
		lastTicks = ticks;

		// update rotation
		r += M_PI / 2.0f * time;

		// display frames per second
		unsigned int fps = 1000 / ticksDiff;
		//printf("Frames per second: %u    \r", fps);

	}	// End of while(isRun)

	clear();


}

// PRIVATE METHODS

// Called from constructors. Generate and set class members
void EngineWindow::setMembers(const char* title, int x, int y, int w, int h, Uint32 flags)
{
	// Get display window and frame buffer
	mWindow = SDL_CreateWindow(title, x, y, w, h, flags);	// Create window

	if (mWindow == NULL)	// Window creation failed!
	{
		std::cout << "Failed at window creation! SDL_ERROR: " << SDL_GetError() << std::endl;

		// TODO	WRITE LOG FILE

		return;
	}

	mSurface = SDL_GetWindowSurface(mWindow);

	mRasterizer = Rasterizer((uint32_t *)mSurface->pixels, w, h);

	isRun = false;
	currentSceneIndex = 0;

	mLast = 0;
	mDeltaTime = 0.f;

	mMouseOldX = mMouseOldY = 0;

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	//mFrameBuffer = SDL_GetWindowSurface(mWindow);	// Get frame buffer
}

// Called when event occured. Calls related event handler
void EngineWindow::handleEvents(const SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		isRun = false;
		break;
	case SDL_KEYDOWN:
		handleKeyDownEvents(event);
		break;
	case SDL_KEYUP:
		handleKeyUpEvents(event);
		break;
	/*case SDL_MOUSEMOTION:
		handleMouseMotion(event);
		break;*/
	}

}	// End of handleEvents()

// Handle KEY DOWN events
void EngineWindow::handleKeyDownEvents(const SDL_Event &event)
{

	// TODO MAKE THIS BETTER
	vec3 FORWARD = vec3(0, 0, 1);
	glm::vec3 BACKWARD(0, 0, -1);
	glm::vec3 LEFT(1, 0, 0);
	glm::vec3 RIGHT(-1, 0, 0);

	glm::vec3 DOWN(0, -1, 0);
	glm::vec3 UP(0, 1, 0);

	glm::vec3 ONE(1, 1, 1);
	glm::vec3 ZERO(0, 0, 0);

	switch (event.key.keysym.sym)
	{
	case SDLK_w:
		mScene.moveCamera(FORWARD, mDeltaTime);
		break;
	case SDLK_s:
		mScene.moveCamera(BACKWARD, mDeltaTime);
		break;
	case SDLK_a:
		mScene.moveCamera(LEFT, mDeltaTime);
		break;
	case SDLK_d:
		mScene.moveCamera(RIGHT, mDeltaTime);
		break;
	}


}	// End of handleKeyEvents()

// Handle key events
void EngineWindow::handleKeyUpEvents(const SDL_Event &event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_r:
		std::cout << "R\n";
		//SDL_FillRect(mFrameBuffer, NULL, SDL_MapRGB(mFrameBuffer->format, 255, 0, 0));
		//SDL_UpdateWindowSurface(mWindow);	// Update the window
		break;
	case SDLK_b:
		std::cout << "B\n";
		//SDL_FillRect(mFrameBuffer, NULL, SDL_MapRGB(mFrameBuffer->format, 0, 0, 255));
		//SDL_UpdateWindowSurface(mWindow);	// Update the window
		break;
	case SDLK_ESCAPE:
		isRun = false;
		break;
	}


}	// End of handleKeyEvents()

// HandleMOUSE MOTION events
// TODO STH WRONG!
void EngineWindow::handleMouseMotion(const SDL_Event &event)
{
	// TODO DO IT BETTER
	float rY = 0, rX = 0;
	int x = event.motion.x; int y = event.motion.y;

	rY += (y - mMouseOldY) / 5.0f;
	rX += (mMouseOldX - x) / 5.0f;

	mMouseOldX = x;
	mMouseOldY = y;

	mScene.rotateCamera(rX, rY, 0);
}

// Stop running and clear
void EngineWindow::clear()
{
	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
}