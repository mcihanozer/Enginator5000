#include "MeshReader.h"

#include "Common.h"

#include <stdio.h>

struct ObjFace
{
	ObjFace(const int* vertexIn, const int* normIn)
	{
		smVertIndex[0] = vertexIn[0];
		smVertIndex[1] = vertexIn[1];
		smVertIndex[2] = vertexIn[2];

		smNormIndex[0] = normIn[0];
		smNormIndex[1] = normIn[1];
		smNormIndex[2] = normIn[2];
	}

	ObjFace(int v1in, int v2in, int v3in, int n1in, int n2in, int n3in )
	{
		smVertIndex[0] = v1in;
		smVertIndex[1] = v2in;
		smVertIndex[2] = v3in;

		smNormIndex[0] = n1in;
		smNormIndex[1] = n2in;
		smNormIndex[2] = n3in;
	}

	int smVertIndex[3];
	int smNormIndex[3];
};


// PUBLIC METHODS

// Calls correct loader depending on mesh type
bool MeshReader::loadMesh(const char* type, const char* path, Mesh* mesh)
{
	if (strcmp("OBJ", type) == 0 || strcmp("obj", type) == 0)	// Load OBJ file
	{
		FILE* filePtr = fopen(path, "r");
		
		//std::ifstream filePtr(path, std::ios::in);	// Open the file

		if (filePtr)
		{
			loadObj(filePtr, mesh);
			fclose(filePtr);
			return true;
		}
		else
		{
			printf("ERROR DURING OPEN OBJ FILE: %s", path);
		}
	}
	else if (strcmp("SMD", type) == 0 || strcmp("smd", type) == 0)	// Load SMD file
	{
		// LOAD SMD FILE
	}

	return false;
}

// PRIVATE METHODS

// Loads OBJ file into a given mesh
bool MeshReader::loadObj(FILE* file, Mesh* mesh)	//(std::ifstream& file, Mesh* mesh)
{
	//int faceIndex = 0;	// To keep track of faces
	char line[4096];	// To read lines in the OBJ file

	std::vector<glm::vec3> vertPosList;
	std::vector<glm::vec3> vertNormList;
	std::vector<ObjFace> faceList;

	std::vector<string> tokens;

	//int nextNormal = 0;	// Keep track of normals
	bool isUV = false;	// To see whether the file has UVs or not. It's a helper variable for face parsing

	while (fgets(line, sizeof(line), file) != NULL)	// Until end of the file
	{
		// Remove newline character
		if (line[strlen(line) - 1] == '\n')
		{
			line[strlen(line) - 1] = '\0';
		}

		tokenizer(" ", line, tokens);

		if (tokens.size() > 0)
		{
			if (tokens[0].compare("v") == 0)	// Vertex
			{
				// Set to the vertex
				//Vertex newVertex(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

				//mesh->mVert.push_back(newVertex);

				glm::vec3 newPos(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
				vertPosList.push_back(newPos);
			}
			else if (tokens[0] == "vt")	// Texture coordinates
			{
				isUV = true;
			}
			else if (tokens[0].compare("vn") == 0)	// Vertex normal
			{
				// Set normal of the next vertex
				glm::vec3 newNormal(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
				vertNormList.push_back(glm::normalize(newNormal));


				/*mesh->mVert[nextNormal].mNormal.x = std::stof(tokens[1]);
				mesh->mVert[nextNormal].mNormal.y = std::stof(tokens[2]);
				mesh->mVert[nextNormal].mNormal.z = std::stof(tokens[3]);*/

				// Normalize the mesh normal
				//mesh->mVert[nextNormal].mNormal = glm::normalize(mesh->mVert[nextNormal].mNormal);

				//nextNormal++;	// Get ready for the next one
			}
			else if (tokens[0].compare("f") == 0)	// Face
			{
				int faceIndices[4] = { -1, -1, -1, -1 };
				int normIndices[4] = { -1, -1, -1, -1 };

				std::vector<string> faceResult;

				// TODO THIS ONLY WORKS FOR FACES CONTAINING UP TO 4 VERTICES
				for (int fi = 1; fi < tokens.size(); fi++)
				{
					tokenizer("/", tokens[fi].c_str(), faceResult);	// Remove "/" char and get the information

					faceIndices[fi - 1] = stoi(faceResult[0]) - 1;
					
					if (isUV)
					{
						// TODO HANDLE UVs
					}
					else
					{
						
					}

					// TODO HANDLE IF FACE DOES INCLUDE JUST VERTEX INFO X/X/X YERINE X
					normIndices[fi - 1] = stoi(faceResult[1]) - 1;

					faceResult.clear();
				}

				// TODO Remove duplicated vertices

				// Set for condition for generating new vertices. It also uses for setting faces
				int vertNum = -1;
				if (tokens.size() > 4)	// which means the face defined with 4 vertices
				{
					vertNum = 4;
				}
				else
				{
					vertNum = 3;
				}

				//// Set vertices to do mesh
				//for (int i = 0; i < vertNum; i++)
				//{
				//	Vertex newVert(vertPosList[faceIndices[i]], vertNormList[normIndices[i]]);

				//	mesh->mVert.push_back(newVert);
				//}

				// Set mesh face list
				if (vertNum == 3)	// Regular triangle
				{
					ObjFace newFace(faceIndices, normIndices);

					faceList.push_back(newFace);
				}
				else	// It's a quad, need to make 2 triangles
				{
					ObjFace newFaceI(faceIndices[0], faceIndices[1], faceIndices[2], normIndices[0], normIndices[1], normIndices[2]);
					faceList.push_back(newFaceI);

					ObjFace newFaceII(faceIndices[0], faceIndices[2], faceIndices[3], normIndices[0], normIndices[2], normIndices[3]);
					faceList.push_back(newFaceII);
				}

			}
			else if (tokens[0].compare("g") == 0)	// Group
			{
				int a = 5;	// TODO parse g
			}

			tokens.clear();

		}	// End of if token.size() > 0

	}	// End of file reading while()

	mesh->mFace.resize(faceList.size());
	mesh->mVert.resize(faceList.size() * 3);

	
	for (int i = 0, meshFaceIndex = 0; i < faceList.size(); i++, meshFaceIndex += 3)
	{
		// Set first vertex
		Vertex vert1(vertPosList[faceList[i].smVertIndex[0]], vertNormList[faceList[i].smNormIndex[0]]);
		mesh->mVert[meshFaceIndex] = vert1;

		// Set second vertex
		Vertex vert2(vertPosList[faceList[i].smVertIndex[1]], vertNormList[faceList[i].smNormIndex[1]]);
		mesh->mVert[meshFaceIndex + 1] = vert2;

		// Set third vertex
		Vertex vert3(vertPosList[faceList[i].smVertIndex[2]], vertNormList[faceList[i].smNormIndex[2]]);
		mesh->mVert[meshFaceIndex + 2] = vert3;

		// Set face and face normal
		Face newFace(meshFaceIndex, meshFaceIndex + 1, meshFaceIndex + 2);
		newFace.mFaceNormal = glm::normalize(glm::cross((vert3.mPos - vert1.mPos), (vert2.mPos - vert1.mPos)));
		mesh->mFace[i] = newFace;

	}

	return true;
}