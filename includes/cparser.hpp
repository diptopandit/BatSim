#ifndef  CPARSER_CLASS
#define  CPARSER_CLASS

class cParser
{
	public:
		cParser();
		bool getInput(void);
		bool parseInput(void);
		bool applyInstructionsOn(cBattery&);
	private:
		int CommandNumber;
		int Keynumber;
		double Param[3];
		int NumberofParam;
		int FunctionNumber;

}

#endif //CPARSER_CLASS
