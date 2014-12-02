#ifndef  CPARSER_CLASS
#define  CPARSER_CLASS

#include "battery.hpp"

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
		char command[20];
		char key[20];
		double value[3];
		int CommandNumber;
		int KeyNumber;
		double Param[3];
		int NumberofParam;
		int FunctionNumber;

};

#endif //CPARSER_CLASS
