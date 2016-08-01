#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "Camera.h"

//#include "Mesh.h"
//
//#include "Rasterizer.h"
//
////#include <glm.hpp>
////using glm::vec3;
////using glm::mat4x4;
//
//#include <map>	// For propertyList
//
//#include <string>	// For propertyList
//using std::string;


class PerspectiveCamera : Camera
{
public:
	// CONSTRUCTORS
	PerspectiveCamera()	{}

	PerspectiveCamera(std::map<string, string>& propertyList, Rasterizer* rasterizer);	// Create camera using the properties read from XML file

	// Set camera parameters, and calculate inverse of camera matrix for camera/viewing transformation
	// Up vector is assumed (0, 1, 0)
	// FOV is considered as full, not half. So, half of it should be taken in the code
	PerspectiveCamera(Rasterizer* rasterizer, const vec3& campos, const vec3& lookat, float fov = 90.f, float distN = 1.f, float distF = 3.f, float ratio = 1.33);

	// Set camera parameters, and calculate inverse of camera matrix for camera/viewing transformation
	// Up vector is specified
	// FOV is considered as full, not half. So, half of it should be taken in the code
	PerspectiveCamera(Rasterizer* rasterizer, const vec3& campos, const vec3& lookat, const vec3& camUp, float fov = 45.f, float distN = 0.1f, float distF = 3.f, float ratio = 1.33);

	// METHODS
	// Same thing with Camera(std::map<string, string>& propertyList, Rasterizer* rasterizer) constructor
	// TODO FIND A BETTER SOLUTION FOR THIS
	void setFromPropList(std::map<string, string>& propertyList, Rasterizer* rasterizer);

	// Move camera
	void moveCamera(const glm::vec3& direction, float deltaTime);

	// Rotate Camera
	void rotateCamera(const float yaw, const float pitch, const float roll);

	// Transform vertices of the given mesh list to the camera space
	void transformToCameraSpace(const std::vector<Mesh*>& sourceMeshes, std::vector<TransformedMesh>& transMesh); //std::vector<std::vector<Vertex>>& transformedVerts);

	void draw(const std::vector<TransformedMesh>& transMesh); //(const std::vector<Mesh*>& sourceMeshes, const std::vector<std::vector<Vertex>>& verts);	// Draw the scene

	// PUBLIC MEMBERS
	
	// World Space ------> Camera Space
	// mat4x4 and mat4 is the same thing: https://www.opengl.org/registry/doc/GLSLangSpec.4.20.8.pdf
	// 4.1.6 Matrices, page 26: "mat4x4 view; // an alternate way of declaring a mat4",
	// "Matrix types beginning with "mat" have single-precision components ... The first number in the type is the number of columns,
	// the second is the number of rows.If there is only one number, the matrix is square."
	mat4x4 mInvCamMatrix;	// Inverse camera matrix (invC) for camera/viewing transformation

	mat4x4 mProjectionMatrix;	// Paralel projection matrix

private:

	// PRIVATE METHODS
	void setCamera(Rasterizer* rasterizer, const vec3& campos, const vec3& lookat, const vec3& camUp, float fov, float distN, float distF, float ratio);

	// MEMBERS
	float mfov;		// Field of view (FOV)
	//float mdNear;	// Distance to near plane
	//float mdFar;	// Distance to far plnae
	float mRatio;	// Ratio between the horizontal and vertical fields of view

	glm::vec4 mTranslation;

	//vec3 mCamPos;	// Camera position
	//vec3 mLookAt;	// Camera look at

	//Rasterizer* mRasterizer;	// Keeps frame buffer and handles setting pixel colors

};

#endif