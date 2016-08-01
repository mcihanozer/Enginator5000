#ifndef ENGINATOR5000_H
#define ENGINATOR5000_H

#include "EngineWindow.h"

class Enginator5000
{
public:
	enum xmlNodes
	{
		eUnknown,
		eWindow,
		eCamera,
		eMesh
	};


	// CONSTRUCTOR
	Enginator5000();

	// METHODS
	// Loads the given scene file (.xml)
	// TRUE: Scene loaded succesfully, FALSE: Faced with a problem
	bool loadScene(const char* scenePath);

	

private:
	// PRIVATE METHODS
	
	xmlNodes getXmlEnum(const char* nodeName);	// Helper method for using XML node strings in swtich() in loadScene() method
	// Puts all properties fo a given node
	//void getPropertyList(rapidxml::xml_attribute<>* nextAttr, std::map<string, string> & propertyList);

	// MEMBERS
	EngineWindow* mWindow;

};

#endif