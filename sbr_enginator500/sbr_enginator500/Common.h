#ifndef COMMON_H
#define COMMON_H

/*
This header keeps helper/common structures, functions etc. needed for most of the classes
*/

#include <string>	// For FileList structure
using std::string;

#include <vector>	// For tokenizing

// DATA STRUCTURES

struct FileList
{
	string name;
	string path;
};

// EXTERN FUNCTIONS

// Takes the line and return each token
extern void tokenizer(const char* delimeters, const char* line, std::vector<string>& tokens);

// Takes a string, returns first characters until whitespace in prefix, and the rest in rest
extern void tokenWhiteSpace(string& line, char* prefix, string& rest);

// Takes a string and returns the character serie after the first whitespace
extern void getFirstChars(string& line, char* prefix);

// Takes a string and returns 2 int values
extern void strToVec2(string& input, float* values);

// Takes a string and returns 3 int values
extern void strToVec3(string& input, float* values);

#endif