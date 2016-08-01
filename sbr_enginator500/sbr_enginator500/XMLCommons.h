#ifndef XMLCOMMONS_H
#define XMLCOMMONS_H

/*
This header keeps helper/common structures, functions etc. needed XML parsing process
*/

#include <map>	// For returning XML node attributes
#include <string>	// char* did not work with std::map

#include <rapidxml.hpp>	// For reading XML file

using std::string;

// Reads all properties of a given XML attribute and returns all valus in a std::map
extern void getProperties(rapidxml::xml_attribute<>* nextAttr, std::map<string, string>& propertyList);

// Read the XML file with a given path and return root node
extern void getRootNode(const char* filePath, const char* nodeName, rapidxml::xml_document<>& doc);	// , rapidxml::xml_node<>* rootNode

#endif