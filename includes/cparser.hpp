/**
 * @file cparser.hpp
 * @brief Defines the command parser class
 * 
 * The command parser takes list of valod keywords and commands
 * Upon input it parses the input, tokenize it and returns the function number 
 * corresponding to particular command- key combination
 * @author Dipta Pandit
 * @date 3/12/2014
 * @see cparser.cpp
 */
#ifndef  CPARSER_CLASS
#define  CPARSER_CLASS

class cParser
{
	public:
		cParser();
		bool getInput(void);
		bool parseInput(const char**, const char**);
		int getFunctionNumber(void);
		char* getLastCommand(void);
		char* getLastKey(void);
		double getParam(int);
		int getParamCount(void);
	private:
		char command[20];	///<Holds the command
		char key[20];		///<Holds the key
		double value[3];    ///<Holds the initial input parameters
		int CommandNumber;  ///<Index of the command
		int KeyNumber;      ///<Index of the key
		double Param[3];    ///<Holds the parameters (3 at max)
		int NumberofParam;  ///<Holds the number of parameter
		int FunctionNumber; ///<Unique function number from command - key combination
};

#endif //CPARSER_CLASS
