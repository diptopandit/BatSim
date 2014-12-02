#include "../includes/cparser.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>

cParser::cParser()
{
}

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

bool cParser::parseInput(const char** validCommands, const char** validKeys)
{
	if(NumberofParam <1)
		return false;
	int i;
	CommandNumber = 0;
	KeyNumber = 0;
	for (i=0;validCommands[i] != (char*)0;i++)
	{
		if(!strcmp(validCommands[i],command))
			break;
	}
	CommandNumber = i;
	if(NumberofParam > 1)
	{
		for (i=0;validKeys[i] != (char*)0;i++)
		{
			if(!strcmp(validKeys[i],key))
				break;
		}
		KeyNumber = i;
	}
	for(i=0;i<NumberofParam-2;i++)
		Param[i] = value[i];
	FunctionNumber = CommandNumber * 100 + KeyNumber;
	return true;
}

char* cParser::getLastCommand(void)
{
	return command;
}

char* cParser::getLastKey(void)
{
	return key;
}

double cParser::getParam(int index)
{
	return Param[index];
}

int cParser::getFunctionNumber(void)
{
	return FunctionNumber;
}

int cParser::getParamCount(void)
{
	if((NumberofParam - 2)<0)
		return 0;
	return NumberofParam - 2;
}
