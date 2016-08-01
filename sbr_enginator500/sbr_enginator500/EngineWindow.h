#ifndef ENGINEWINDOW_H
#define ENGINEWINDOW_H

#include "Scene.h"
#include "Rasterizer.h"

#include <vector>	// For FileList

#include <map>	// For propertyList

class EngineWindow	// TODO MAKE AN ABSTRACT WINDOW CLASS
{
public:
	// CONSTRUCTOR
	EngineWindow(std::map<string, string>& propertyList, std::vector<FileList> sceneList);	// Create Enginator5000 window for display using the properties read from XML file
	EngineWindow(SDL_WindowFlags flags = SDL_WINDOW_SHOWN);	// Create Enginator5000 window for display: Creates	1024x768 centered window
	EngineWindow(int width, int height, SDL_WindowFlags flags = SDL_WINDOW_SHOWN);	// Create Enginator5000 window for display: Create centered window with given dimension
	EngineWindow(int winPosX, int winPosY, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_SHOWN);	// Create Enginator5000 window for display with given position and dimension

	//  PUBIC METHODS
	void init();	// Initialize elements: Read and set next scene

	void run();	// Start running: Takes events, refresh the buffers etc.

	void runLineExample();	// Run line drawing example


private:
	// PRIVATE METHODS
	void setMembers(const char* title, int x, int y, int w, int h, Uint32 flags);	// Called from constructors. Generate and set class members

	void handleEvents(const SDL_Event &event);		// Called when event occured. Calls related event handler

	void handleMouseMotion(const SDL_Event &event);	// HandleMOUSE MOTION events

	void handleKeyDownEvents(const SDL_Event &event);	// Handle KEY DOWN events
	void handleKeyUpEvents(const SDL_Event &event);	// Handle KEY UP events

	void clear();	// Stop running and clear

	// MEMBERS
	bool isRun;	// For keep running
	int currentSceneIndex;	// Keep track of the scene. Default values is 0 (First scene in the mSceneList)

	long mLast;	// Last time (for delta-time)
	float mDeltaTime;	// Delta-time for movement

	int mMouseOldX, mMouseOldY;

	SDL_Window*		mWindow;	// SDL_Window
	SDL_Surface*	mSurface;	// Surface of the window

	std::vector<FileList> mSceneList;

	Scene mScene;
	Rasterizer mRasterizer;	// Keeps frame buffer and handles setting pixel colors

};

#endif