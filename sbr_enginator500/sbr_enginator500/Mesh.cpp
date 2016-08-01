#include "Mesh.h"

/*****************************************/
//				VERTEX CLASS			 //
/*****************************************/

// Generate a new vertex by setting position
Vertex::Vertex(const float x, const float y, const float z)
{
	mPos.x = x;
	mPos.y = y;
	mPos.z = z;
}

// Generate a new vertex from given a position
Vertex::Vertex(const glm::vec3& pos)
{
	mPos.x = pos.x;
	mPos.y = pos.y;
	mPos.z = pos.z;
}

// Generate a new vertex from given position and normal
Vertex::Vertex(const glm::vec3& pos, const glm::vec3& normal)
{
	mPos.x = pos.x;
	mPos.y = pos.y;
	mPos.z = pos.z;

	mNormal.x = normal.x;
	mNormal.y = normal.y;
	mNormal.z = normal.z;
}

// Generate a new vertex from given vertex
Vertex::Vertex(const Vertex& sourceVert)
{
	mPos = sourceVert.mPos;
	mNormal = sourceVert.mNormal;
	mUV = sourceVert.mUV;
}

// OPERATOR OVERLOADINGS

// Copy source vertex
void Vertex::operator= (const Vertex& sourceVert)
{
	mPos = sourceVert.mPos;
	mNormal = sourceVert.mNormal;
	mUV = sourceVert.mUV;
}

/*****************************************/
//				MESH CLASS				//
/*****************************************/

void Mesh::applyTransformation()
{
	for (int i = 0; i < mVert.size(); i++)
	{
		mVert[i].mPos = vec3(mTransformation * glm::vec4(mVert[i].mPos, 1.f));
	}
}


//Mesh::Mesh(MeshType type, const char* filePath)
//{
//	mType = type;
//	mFilePath = filePath;
//}