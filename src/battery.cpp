#include "../includes/battery.hpp"
#include <unistd.h>
#include <iostream>

cBattery::cBattery()
{
	count = 0;
	Vout = 0;
	Iout = 0;
	ElapsedTime = 0;
	CutOffVoltage = 7;
	SimulatorState.unlock();
	for(int i=0; i<3; i++)
		Switch[i] = false;
}

bool cBattery::getSwitchState(int cell)
{
	bool result;
	AccessSynchroniser.lock();
	result = Switch[cell];
	AccessSynchroniser.unlock();
	return result;
}

double cBattery::getElapsedTime(void)
{
	double result;
	AccessSynchroniser.lock();
	result = ElapsedTime;
	AccessSynchroniser.unlock();
	return result;
}

bool cBattery::reset(void)
{
	bool status = false;
	for(int i=0; i<3; i++)
	{
		status = Cell[i]->lock(this);
		if(status)
		{
			status = Cell[i]->loadDefaults(this);
			Cell[i]->unlock(this);
		}
	}
	return status;
}

bool cBattery::run(double load,double resolution,double speed)
{
	if(IsRunning())
		return false;
	bool status = false;
	status = Cell[0]->lock(this);
	if(status) status = Cell[1]->lock(this);
	if(status) status = Cell[2]->lock(this);
	if(!status)
		return false;
	SimulatorState.lock();
	Runner = new std::thread(&cBattery::runBattery, this, load, resolution, speed);
	return true;
}

bool cBattery::stop(void)
{
	if(IsRunning())
	{
		SimulatorState.unlock();
		Runner->join();
		bool status = false;
		status = Cell[0]->unlock(this);
		if(status) status = Cell[1]->unlock(this);
		if(status) status = Cell[2]->unlock(this);
		return status;
	}
	return false;
}

bool cBattery::addCell(cCell* AdCell)
{
	if(count>=3)
		return false;
	Cell[count++] = AdCell;
	return true;
}

double cBattery::getVout(void)
{
	double result;
	AccessSynchroniser.lock();
	result = Vout;
	AccessSynchroniser.unlock();
	return result;
}

double cBattery::getIout(void)
{
	double result;
	AccessSynchroniser.lock();
	result = Iout;
	AccessSynchroniser.unlock();
	return (result*1000);
}

bool cBattery::IsRunning(void)
{
	return ContinueRunning();
}

bool cBattery::ContinueRunning(void)
{
	if(SimulatorState.try_lock())
	{
		SimulatorState.unlock();
		return false;
	}
	return true;
}

void cBattery::runBattery(double load, double resolution, double speed)
{
	int i;
	double OutVolt=0;
	double divisor = 0;
	bool change = false;
	bool localSwitch[3] = {false,false,false};

	FILE* logFile;

	logFile = fopen("./batsim.log","a");


	fprintf(logFile,"**************************************************\n");
	fprintf(logFile,"\t\t\tBattery Simulator\n");
	fprintf(logFile,"***************************************************\n");
	fprintf(logFile,"\n[%9.3f]\tSimulator Started\n",ElapsedTime/1000);

	while(ContinueRunning())
	{

		fprintf(logFile,"\n[%9.3f]\tOutVolt: %f\tIout: %f\n\tCell 1:: %d: %f V,\t%f mA\n\tCell 2:: %d: %f V,\t%f mA\n\tCell 3:: %d: %f V,\t%f mA\n",
			ElapsedTime/1000,Vout,Iout*1000,Switch[0],Cell[0]->getCurrentVoltage(),Cell[0]->getSourceCurrent()*1000,Switch[1],Cell[1]->getCurrentVoltage(),Cell[1]->getSourceCurrent()*1000,Switch[2],Cell[2]->getCurrentVoltage(),Cell[2]->getSourceCurrent()*1000);

		for(i=0;i<3;i++)
			localSwitch[i] = true;
		do
		{
			change = false;
			OutVolt = 0;
			divisor =0;
			for (i=0;i<3;i++)
			{
				if(localSwitch[i])
				{
					OutVolt += ((Cell[i]->getCurrentVoltage())/Cell[i]->getSeriesResistance());
					divisor += (1/Cell[i]->getSeriesResistance());
				}
			}
			divisor += (1/load);
			OutVolt = OutVolt/divisor;
			for(i=0;i<3;i++)
			{
				if(localSwitch[i] && (Cell[i]->getCurrentVoltage() < OutVolt))
				{
					localSwitch[i] = false;
					change = true;
				}
			}
		}while(change);

		//sleep for Inteval
		usleep(resolution*1000/speed);
		ElapsedTime += resolution;

		//if total voltage < MIN, break;
		if(OutVolt < CutOffVoltage)
		{
			for(i = 0; i<3; i++)
				localSwitch[i] = false;
			SimulatorState.unlock();
			std::cout<<"\nBattery exhausted\nSimulation completed\n";
			fprintf(logFile,"ALERT :: exhausted\n");
			std::cout<<"BatSim >> ";
		}

		//update out voltage
		Vout = OutVolt;

		Iout = 0;

		for(i = 0; i<3; i++)
		{
			if(localSwitch[i])
			{
				Cell[i]->update(Vout,resolution);
				//update total current
				Iout+= Cell[i]->getSourceCurrent();
			}
			//update the switch
			Switch[i] = localSwitch[i];
		}

	}
	fprintf(logFile,"\n[%9.3f]\tSimulator Stopped\n",ElapsedTime/1000);
	fcloseall();
	return;
}
