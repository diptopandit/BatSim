#include "../includes/simulator.hpp"
#include <iostream>

cBatSim::cBatSim(void)
{
	Resolution = 100;
	Speed = 1;
	BatteryRunning = false;
}

cBatSim::cBatSim(int multiplier, double res)
{
	if(0 == res)
		Resolution = 100;
	else
		Resolution = res;
	if(0 == multiplier)
		Speed = 1;
	else
		Speed = multiplier;
}

bool cBatSim::start(void)
{
	if(BatteryRunning)
		return false;
	std::cout<<"calling battery run"<<std::endl;
	BatteryRunning = BatPack->run(Load,Resolution,Speed);
	return BatteryRunning;
}

bool cBatSim::stop(void)
{
	if(!BatteryRunning)
		return false;
	if(BatPack->stop())
		BatteryRunning = !BatPack->reset();
	return !BatteryRunning;
}

bool cBatSim::pause(void)
{
	if(!BatteryRunning)
		return false;
	BatteryRunning = BatPack->stop();
	return BatteryRunning;
}

bool cBatSim::resume(void)
{
	return start();
}

bool cBatSim::setSpeed(int multiplier)
{
	if(BatteryRunning)
		return false;
	if(0 == multiplier)
		return false;
	Speed = multiplier;
	return true;
}

bool cBatSim::setResolution(double milisec)
{
	if(BatteryRunning)
		return false;
	if(0 == milisec)
		return false;
	Resolution = milisec;
	return true;
}

bool cBatSim::connect(cBattery* battery)
{
	if(BatteryRunning)
		return false;
	BatPack = battery;
	return true;
}

bool cBatSim::connect(double load)
{
	if(BatteryRunning)
		return false;
	Load = load;
	return true;
}
