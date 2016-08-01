#include "Scene.h"

#include "XMLCommons.h"

#include <fstream>	// XML files first reads into a <char> vector. ifstream used for this operation
#include <vector>	// XML files first reads into a <char> vector

#include <glm\gtx\euler_angles.hpp>	// For setting up rotation matrix form euler angles (XYZ)
#include <glm\gtc\matrix_transform.hpp>	// For setting up scale, and translate matrices

// CONSTRUCTOR
// Set up scene members
Scene::Scene()
{

}

// PUBLIC METHODS
// Load a new scene
bool Scene::loadScene(const char* filePath, Rasterizer* rasterizer)
{
	delete mCamera;

	// Read the first scene file
	std::ifstream fileReader(filePath);	// Open file

	if (fileReader)	// Ready to go
	{
		std::vector<char> buffer((std::istreambuf_iterator<char>(fileReader)), (std::istreambuf_iterator<char>()));	// Iterate from the first elemen to end.
																			// When we reach he end of stream, the iterator becomes equivalent to an iterator 
																			// created using the default constructor: std::istreambuf_iterator<char>()
		buffer.push_back('\0');
		fileReader.close();

		// Start parsing
		rapidxml::xml_document<> doc;
		doc.parse<0>(&buffer[0]);

		rapidxml::xml_node<>* rootNode = doc.first_node("scene");	// Get root nonde: <game>

		// Read camera properties
		std::map<string, string> camPropList;
		rapidxml::xml_node<>* cameraNode = rootNode->first_node();
		getProperties(cameraNode->first_attribute(), camPropList);

		// Init camera
		mCamera = new PerspectiveCamera(camPropList, rasterizer);	//Camera(camPropList, rasterizer);

		// Read node list
		// TODO NODE TRANSFORMS!
		// TODO MESH READER: ONCE OBJ DENE

		rapidxml::xml_node<>* instanceNodes = cameraNode->next_sibling();	// Get first scene in the scene list
		std::vector<FileList> meshList;

		while (instanceNodes)	// Traverse all <scene>s and put into the vector
		{
			// Instancing
			mat4x4 instanceTransformation;

			if (instanceNodes->first_attribute())	// if instance has attributes
			{
				// Get scale
				mat4x4 inScaleMatrix;
				if (instanceNodes->first_attribute("scale"))
				{
					string insS = instanceNodes->first_attribute("scale")->value();
					float insScaleValues[3];
					strToVec3(insS, insScaleValues);
					vec3 insScale(insScaleValues[0], insScaleValues[1], insScaleValues[2]);
					inScaleMatrix = glm::scale(glm::mat4(1.f), insScale);
				}

				// Get rotation
				mat4x4 insRotationMatrix;
				if (instanceNodes->first_attribute("rotation"))
				{
					string insS = instanceNodes->first_attribute("rotation")->value();
					float insEulerAngles[3];
					strToVec3(insS, insEulerAngles);
					insRotationMatrix = glm::eulerAngleXYZ(insEulerAngles[0], insEulerAngles[1], insEulerAngles[2]);
				}

				// Get translation
				mat4x4 insTranslationMatrix;
				if (instanceNodes->first_attribute("translation"))
				{
					string insS = instanceNodes->first_attribute("translation")->value();
					float insTranslationValues[3];
					strToVec3(insS, insTranslationValues);
					vec3 insTranslation(insTranslationValues[0], insTranslationValues[1], insTranslationValues[2]);
					insTranslationMatrix = glm::translate(glm::mat4(1.f), insTranslation);
				}

				// Set instance matrix
				instanceTransformation = insTranslationMatrix * insRotationMatrix * inScaleMatrix;

			}	// End of if instance has attributes

			if (instanceNodes->first_node())
			{
				rapidxml::xml_node<>* nextMeshNode = instanceNodes->first_node();

				while (nextMeshNode)
				{
					// Read mesh
					Mesh* newMesh = new Mesh();

					MeshReader::loadMesh(nextMeshNode->first_attribute("type")->value(), nextMeshNode->first_attribute("path")->value(), newMesh);

					mMeshList.push_back(newMesh);

					// Set transformation
					if (nextMeshNode->first_node("transform"))	// if mesh has transformation node
					{
						rapidxml::xml_node<>* transformationNode = nextMeshNode->first_node("transform");

						// Get scale
						mat4x4 scaleMatrix;
						if (transformationNode->first_attribute("scale"))
						{
							string s = transformationNode->first_attribute("scale")->value();
							float scaleValues[3];
							strToVec3(s, scaleValues);
							vec3 scale(scaleValues[0], scaleValues[1], scaleValues[2]);
							scaleMatrix = glm::scale(glm::mat4(1.f), scale);
						}

						// Get rotation
						mat4x4 rotationMatrix;
						if (transformationNode->first_attribute("rotation"))
						{
							string s = transformationNode->first_attribute("rotation")->value();
							float eulerAngles[3];
							strToVec3(s, eulerAngles);
							rotationMatrix = glm::eulerAngleXYZ(eulerAngles[0], eulerAngles[1], eulerAngles[2]);
						}

						// Get translation
						mat4x4 translationMatrix;
						if (transformationNode->first_attribute("translation"))
						{
							string s = transformationNode->first_attribute("translation")->value();
							float translationValues[3];
							strToVec3(s, translationValues);
							vec3 translation(translationValues[0], translationValues[1], translationValues[2]);
							translationMatrix = glm::translate(glm::mat4(1.f), translation);
						}

						// Set mesh transformation
						// MODEL = TRANSLATION * ROTATION * SCALE
						newMesh->mTransformation = instanceTransformation * translationMatrix * rotationMatrix * scaleMatrix;

					}	// End of if mesh has transformation node

					//// TODO GET MATERIAL

					nextMeshNode = nextMeshNode->next_sibling();

				}	// End of while(nextMeshNode)

			}	// End of if(meshNodes->first_node())

			// Go for other instance
			instanceNodes = instanceNodes->next_sibling();

		}	// End of while(nextNode)


		return true;
	}

	return false;
}

void Scene::moveCamera(const glm::vec3& direction, float deltaTime)
{
	mCamera->moveCamera(direction, deltaTime);
}

void Scene::rotateCamera(const float yaw, const float pitch, const float roll)
{
	mCamera->rotateCamera(yaw, pitch, roll);
}

// Update meshes (Animation etc.), camera position, related matrices etc.
void Scene::updateScene()
{
	// Prepare copy structure
	std::vector<TransformedMesh> transMesh(mMeshList.size());

	// Get copies
	//std::vector<std::vector<Vertex>> transformedVerts(mMeshList.size());
	//for (unsigned int i = 0; i < mMeshList.size(); i++)
	//{
	//	transformedVerts[i].resize(mMeshList[i]->mVert.size());
	//}

	// Make skinning etc.

	// Make camera transform
	//mCamera->transformToCameraSpace(mMeshList, transformedVerts);
	mCamera->transformToCameraSpace(mMeshList, transMesh);

	// Draw
	//mCamera->draw(mMeshList, transformedVerts);
	mCamera->draw(transMesh);
}