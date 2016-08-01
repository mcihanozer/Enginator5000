#ifndef MESH_H
#define MESH_H

#define GLM_SWIZZLE	// For vec4.xyz()
#include <glm\glm.hpp>	// For vec2 and vec3 in Vertex class

#include <vector>

using glm::vec3;
using glm::vec2;
using glm::mat4x4;

// Supported mesh types
enum MeshType
{
	eOBJ,
	eSMD
};

// Vertices of the mesh
class Vertex
{
public:
	// CONSTRUCTORS
	Vertex()	{}
	Vertex(const float x, const float y, const float z);	// Generate a new vertex by setting position
	Vertex(const glm::vec3& pos);	// Generate a new vertex from given a position
	Vertex(const glm::vec3& pos, const glm::vec3& normal);	// Generate a new vertex from given position and normal
	Vertex(const Vertex& sourceVert);	// Generate a new vertex from given vertex

	// OPERATOR OVERLOADINGS
	void operator= (const Vertex& sourceVert);	// Copy source vertex

	// METHODS


	// MEMBERS
	vec3 mPos;	// Vertex positionm
	vec3 mNormal;	// Vertex normal
	vec2 mUV;	// UV coordinates of the texture

};

// Faces of the mesh
class Face
{
public:
	// CONSTRUCTORS
	Face()	{}
	Face(int i, int j, int k)
	{
		mIndex[0] = i;
		mIndex[1] = j;
		mIndex[2] = k;
	}

	// MEMBERS
	int mIndex[3];	// Kepp vertex indices of the face

	vec3 mFaceNormal;

};

// Structer used after culling etc.
struct TransformedMesh
{
	std::vector<Vertex> mVert;	// Vertex list of the mesh
	std::vector<Face> mFace;	// Face list of the mesh

};

// Keeps mesh related information
class Mesh
{
public:
	// CONSTRUCTORS
	//Mesh(MeshType type, const char* filePath);

	// METHODS
	void applyTransformation();


	// PUBLIC MEMBERS
	mat4x4 mTransformation;

	std::vector<Vertex> mVert;	// Vertex list of the mesh
	std::vector<Face> mFace;	// Face list of the mesh

private:
	// PRIVATE MEMBERS
	//MeshType mType;
	//const char* mFilePath;

};

#endif