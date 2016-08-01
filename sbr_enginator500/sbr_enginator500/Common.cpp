#include "Common.h"

// Takes the line and return each token
extern void tokenizer(const char* delimeters, const char* line, std::vector<string>& tokens)
{
	char token[255] = "\0";
	int tokenIndex	= 0;
	int index = 0;
	bool isToken = false;

	// Remove delimeter above of the line
	for (int deli = 0; deli < strlen(delimeters); deli++)
	{
		while (line[index] == delimeters[deli])
		{
			index++;
		}
	}


	// Check the each character of the line
	for (index; index < strlen(line); index++)
	{
		for (int deli = 0; deli < strlen(delimeters); deli++)
		{
			if (line[index] != delimeters[deli])	// Add characters to the token until find the delimeter
			{
				token[tokenIndex++] = line[index];

				isToken = true;
			}
			else if(isToken)	// Token ends, add into the std::vector
			{
				token[tokenIndex + 1] = '\0';
				tokens.push_back(string(token));

				tokenIndex = 0;
				memset(token, 0, 255);

				isToken = false;
			}
		}	// End of for strlen(delimeters)

	}	// End of for strlen(line)

	if (isToken)
	{
		token[tokenIndex + 1] = '\0';
		tokens.push_back(token);
	}
}


// Takes a string, returns first characters until whitespace in prefix, and the rest in rest
extern void tokenWhiteSpace(string& input, char* prefix, string& rest)
{
	std::size_t firstWS = input.find(" ");	// Get first occurrence of whitespace

	if (string::npos == firstWS)	// If there is no whitespace
	{
		firstWS = input.length();
	}
	else
	{
		std::size_t checker = firstWS;

		while (string::npos == checker)	// Search until last white space
		{
			checker = input.find(" ", checker);
		}
		
		rest = input.substr((checker + 1));
	}

	for (int i = 0; i < firstWS; i++)	// Copy the characters until first whitespace occurrence
	{
		prefix[i] = input[i];
	}

	prefix[firstWS] = '\0';
}

// Takes a string and returns the character serie after the first whitespace
void getFirstChars(string& input, char* prefix)
{
	std::size_t firstWS = input.find(" ");	// Get first occurrence of whitespace

	if (string::npos == firstWS)	// If there is no whitespace
	{
		firstWS = input.length();
	}
	
	for (int i = 0; i < firstWS; i++)	// Copy the characters until first whitespace occurrence
	{
		prefix[i] = input[i];
	}

	prefix[firstWS] = '\0';

}

// Takes a string and returns 2 int values
extern void strToVec2(string& input, float* values)
{
	std::size_t firstWS = input.find(" ");

	// Get first value
	values[0] = stof(input.substr(0, firstWS));

	firstWS = input.find(" ", firstWS + 1);
	while (string::npos != firstWS)	// Search until last white space
	{
		firstWS = input.find(" ", firstWS);
	}
	// Get third value
	values[1] = stof(input.substr(firstWS + 1));
}


// Takes a string and returns 3 int values
void strToVec3(string& input, float* values)
{
	std::size_t firstWS = input.find(" ");
	std::size_t secondWS = input.find(" ", firstWS + 1);

	// Get first value
	values[0] = stof(input.substr(0, firstWS));

	// Get second value
	values[1] = stof(input.substr((firstWS +1), (secondWS - firstWS - 1)));

	// Get third value
	values[2] = stof(input.substr(secondWS + 1));
}