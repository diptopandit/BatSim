#ifndef  SIMULATOR_CLASS
#define  SIMULATOR_CLASS

#include "battery.hpp"

class cBatSim
{
	public:
		cBatSim(void);
		cBatSim(int,double);
		bool start(void);
		bool stop(void);
		bool pause(void);
		bool resume(void);
		bool setSpeed(int);
		bool setResolution(double);
		bool connect(cBattery*);
		bool connect(double);
		double getLoad(void);
	private:
		double Load;
		cBattery* BatPack;
		double Speed;
		double Resolution;
		bool BatteryRunning;
};

#endif //SIMULATOR_CLASS
