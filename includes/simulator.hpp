#ifndef  SIMULATOR_CLASS
#define  SIMULATOR_CLASS

#include "battery.hpp"

class cBatSim
{
	public:
		cBatSim(void);
		cBatSim(int,double);
		~cBatSim(void);
		bool start(void);
		bool stop(void);
		bool pause(void);
		bool resume(void);
		bool setLoad(double);
		bool setSpeed(int);
		bool setResolution(double);
		bool connect(cBattery&);
		bool connect(double);
	private:
		double Load;
		cBattery &BatPack;
		double Speed;
		double Resolution;
};

#endif //SIMULATOR_CLASS
