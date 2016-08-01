#include "Enginator5000.h"

#include "XMLCommons.h"	// For XML reading

#include <fstream>	// XML files first reads into a <char> vector. ifstream used for this operation
#include <vector>	// XML files first reads into a <char> vector

// CONSTRUCTOR

Enginator5000::Enginator5000()
{
	// Load the scene
}

// PUBLIC METHODS

// Loads the given scene file (.xml)
// TRUE: Scene loaded succesfully, FALSE: Faced with a problem
bool Enginator5000::loadScene(const char* scenePath)
{
	// Open Scene file
	std::ifstream fileReader(scenePath);

	//rapidxml::xml_document<> doc;
	//getRootNode(scenePath, "game", doc);
	//rapidxml::xml_node<>* rootNode = doc.first_node();

	if (fileReader.good())	// Ready to go
	{
		std::vector<char> buffer((std::istreambuf_iterator<char>(fileReader)), (std::istreambuf_iterator<char>()));	// Iterate from the first elemen to end.
																			// When we reach he end of stream, the iterator becomes equivalent to an iterator 
																			// created using the default constructor: std::istreambuf_iterator<char>()
		buffer.push_back('\0');
		fileReader.close();

		// Start parsing
		rapidxml::xml_document<> doc;
		doc.parse<0>(&buffer[0]);

		rapidxml::xml_node<>* rootNode = doc.first_node("game");	// Get root nonde: <game>
		rapidxml::xml_node<>* windowNode = rootNode->first_node();	// Get window node

		// Get window attributes
		std::map<string, string> windowPropList;
		getProperties(windowNode->first_attribute(), windowPropList);

		// Get scene list
		rapidxml::xml_node<>* nextNode = windowNode->next_sibling()->first_node();	// Get first scene in the scene list
		std::vector<FileList> sceneList;

		while (nextNode)	// Traverse all <scene>s and put into the vector
		{
			FileList newScene;

			newScene.name = nextNode->first_attribute("name")->value();
			newScene.path = nextNode->first_attribute("path")->value();

			sceneList.push_back(newScene);

			nextNode = nextNode->next_sibling();	// Get next scene

		}	// End of while(nextNode)

		// Generate window
		mWindow = new EngineWindow(windowPropList, sceneList);

		windowPropList.clear();
		sceneList.clear();

		// Start scene creation
		mWindow->init();

		//mWindow->runLineExample();

		mWindow->run();

		return true;	// Reading is done successfuly
	}

	return false;	// Scene file could not open

}	// End of loadScene()

// PRIVATE METHODS

// Helper method for using XML node strings in swtich() in loadScene() method
Enginator5000::xmlNodes Enginator5000::getXmlEnum(const char* nodeName)
{
	if (strcmp("window", nodeName) == 0)
	{
		return xmlNodes::eWindow;
	}
	else if (strcmp("camera", nodeName) == 0)
	{
		return xmlNodes::eCamera;
	}
	else if (strcmp("mesh", nodeName) == 0)
	{
		return xmlNodes::eMesh;
	}

	return xmlNodes::eUnknown;	// If the node is none of the above

}