/**
 * @file cparser.cpp
 * @brief Implementation of command parser class
 * 
 * The command parser takes input from user and tokenize it
 * It can compare the input against a valid command and key list
 * and returns the unique function number
 * @author Dipta Pandit
 * @date 3/12/2014
 * @see cparser.hpp
 */
#include "../includes/cparser.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>

/**
 * @brief Constructor of the cparser object
 * 
 * @param void
 * @return void
 */
cParser::cParser()
{
}

/**
 * @brief Gets input from user
 * 
 * Shows a promp, gets user input and fills the key,
 * command and values.
 * 
 * @param void
 * @return bool true if successfully parses the user input
 * false if failed to get the input or parse it
 */
bool cParser::getInput(void)
{
	char* buffer = new char[100];
	size_t len = 100;
	std::cout<<"batsim>> ";
	NumberofParam = 0;
	command[0] = '\0';
	key[0] = '\0';
	len = getline(&buffer,&len, stdin);
	if(len <= 0)
		return false;
	NumberofParam = sscanf(buffer,"%s%s%lf%lf%lf",command,key,&value[0],&value[1],&value[2]);
	delete []buffer;
	if(NumberofParam < 1)
		return false;
	return true;
}

/**
 * @brief Parse the input
 * 
 * Parse the input and compare the key and command agains a valid set
 * @param const char** valid commands
 * @param const char** valid keys
 * @return bool true if successfully parsed
 * false if number of user inpt parameter is less than 1 or valid command is NULL
 */
bool cParser::parseInput(const char** validCommands, const char** validKeys)
{
	if(NumberofParam <1)
		return false;
	int i;
	CommandNumber = 0;
	KeyNumber = 0;
	if(validCommands == (const char**)0)
		return false;
	for (i=0;validCommands[i] != (char*)0;i++)
	{
		if(!strcmp(validCommands[i],command))
			break;
	}
	CommandNumber = i;
	if(validKeys != (const char**)0)
	{
		if(NumberofParam > 1)
		{
			for (i=0;validKeys[i] != (char*)0;i++)
			{
				if(!strcmp(validKeys[i],key))
					break;
			}
			KeyNumber = i;
		}
	}
	for(i=0;i<NumberofParam-2;i++)
		Param[i] = value[i];
	FunctionNumber = CommandNumber * 100 + KeyNumber;
	return true;
}

/**
 * @brief Returns the last command
 * 
 * @param void
 * @return char* the last command
 */
char* cParser::getLastCommand(void)
{
	return command;
}

/**
 * @brief Returns the last key entered
 * 
 * @param void
 * @return char* The key
 */
char* cParser::getLastKey(void)
{
	return key;
}

/**
 * @brief Returns the specific parameter
 * 
 * Returns the parameter at index 
 * @param int index of the parameter to be returned
 * @return double the parameter at index
 */
double cParser::getParam(int index)
{
	return Param[index];
}

int cParser::getFunctionNumber(void)
{
	return FunctionNumber;
}

/**
 * @brief Returns the parameter count excluding command and key
 * 
 * @param void
 * @return int number of parameter
 */
int cParser::getParamCount(void)
{
	if((NumberofParam - 2)<0)
		return 0;
	return NumberofParam - 2;
}
