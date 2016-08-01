#ifndef MESHREADER_H
#define MESHREADER_H

#include "Mesh.h"

// Class containing static methods for reading given meshes. So far, it supports .OBJ and .SMD files
class MeshReader
{
public:
	// PUBLIC METHODS
	static bool loadMesh(const char* type, const char* path, Mesh* mesh);	// Calls correct loader depending on mesh type

private:
	// PRIVATE METHODS
	static bool loadObj(FILE* file, Mesh* mesh);	//(std::ifstream& file, Mesh* mesh);	// Loads OBJ file into a given mesh
};

#endif