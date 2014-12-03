/**
 * @file simulator.hpp
 * @brief Defines the simulator class
 *
 * The simulator brings up the load and battery pack
 * together to run the battery and observe the changes
 * @author Dipta Pandit
 * @date 3/12/2014
 * @see simulator.cpp
 */
#ifndef  SIMULATOR_CLASS
#define  SIMULATOR_CLASS

#include "battery.hpp"

/**
 * @brief The simulator class
 *
 * Provides an interface to run, stop, pause and resume a battery.
 * Connects the battery with load and maintain speed and resolution of the simulation
 */
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
		double Load;		///<Load to connect with the battery
		cBattery* BatPack;  ///<Pointer to the Battery to be simulated
		double Speed;		///<simulation speed. used to reduce the delay by this factor
		double Resolution;  ///resolution of the simulation. It determines how often battery will be sampled
		bool BatteryConnected;///<denotes weather a battery is connected or not
};

#endif //SIMULATOR_CLASS
