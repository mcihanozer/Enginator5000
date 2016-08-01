#ifndef SCENE_H
#define SCENE_H

#include "Common.h"
#include "PerspectiveCamera.h"	//"Camera.h"
#include "MeshReader.h"

class Scene
{
public:
	// CONSTRUCTOR
	Scene();	// Set up scene members

	// PUBLIC METHODS
	bool loadScene(const char* filePath, Rasterizer* rasterizer);	// Load a new scene
	void updateScene();	// Update meshes (Animation etc.), camera position, related matrices etc.

	void moveCamera(const glm::vec3& direction, float deltaTime);
	void rotateCamera(const float yaw, const float pitch, const float roll);

	std::vector<Mesh*> mMeshList;	// Keeps all meshes in the scene

private:

	// PRIVATE METHODS

	// MEMBERS
	PerspectiveCamera* mCamera;	//Camera* mCamera;	// Camera of the scene

	
};

#endif