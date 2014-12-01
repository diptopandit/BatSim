#include "../includes/simulator.hpp"

cBatSim::cBatSim(void)
{
	Resolution = 100;
	Speed = 1;
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
	//if(running)
		//return false;
	return BatPack->run(Load,Resolution,Speed);
}

bool cBatSim::stop(void)
{
	//if(!running)
		//return false;
	if(BatPack->stop())
		return BatPack->reset();
	return false;
}

bool cBatSim::pause(void)
{
	//if(!running)
		//return false;
	return BatPack->stop();
}

bool cBatSim::resume(void)
{
	return start();
}

bool cBatSim::setSpeed(int multiplier)
{
	//if(running)
		//return false;
	if(0 === multiplier)
		return false;
	Speed = multiplier;
	return true;
}

bool cBatSim::setResolution(double milisec)
{
	//if(running)
		//return false;
	if(0 == milisec)
		return false;
	Resolution = milisec;
	return true;
}

bool cBatSim::connect(cBattery* battery)
{
	//if(running)
		//return fasle;
	BatPack = battery;
	return true;
}

bool cBatSim::connect(double load)
{
	//if(running)
		//return false;
	Load = laod;
	return true;
}
