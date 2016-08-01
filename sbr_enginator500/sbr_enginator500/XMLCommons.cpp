#include "XMLCommons.h"

#include <fstream>	// XML files first reads into a <char> vector. ifstream used for this operation
#include <vector>	// XML files first reads into a <char> vector

// HELPER FUNCTION: Gets all properties for a given node
void getProperties(rapidxml::xml_attribute<>* nextAttr, std::map<string, string>& propertyList)
{
	// TODO Also traverse sub-nodes?

	while (nextAttr)
	{
		propertyList[nextAttr->name()] = nextAttr->value();

		nextAttr = nextAttr->next_attribute();
	}
}

// Read the XML file with a given path and return root node
void getRootNode(const char* filePath, const char* nodeName, rapidxml::xml_document<>& doc)	// , rapidxml::xml_node<>* rootNode
{
	// Open Scene file
	std::ifstream fileReader(filePath);

	if (fileReader)	// Ready to go
	{
		std::vector<char> buffer((std::istreambuf_iterator<char>(fileReader)), (std::istreambuf_iterator<char>()));	// Iterate from the first elemen to end.
		// When we reach he end of stream, the iterator becomes equivalent to an iterator 
		// created using the default constructor: std::istreambuf_iterator<char>()
		buffer.push_back('\0');
		fileReader.close();

		// Start parsing
		//rapidxml::xml_document<> doc;
		doc.parse<0>(&buffer[0]);

		

		//rootNode = doc.first_node(nodeName);

		//return doc.first_node("game");
		
	}
	//else
	//{
	//	rootNode = NULL;
	//}
}