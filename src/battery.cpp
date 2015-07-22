/**
 * @file battery.cpp
 * @brief Implementation of the Battery class
 *
 * A battery consists of one or more cells. Cells are connected in
 * fixed way and the battery behaves acordingly. A battery can be
 * connected to a load and run.
 *
 * @author Dipta Pandit
 * @date 03/12/2014
 * @see battery.h
 */

#include "../includes/battery.hpp"
#include <unistd.h>
#include <iostream> ///<for writing to the log file

/**
 * @brief Constructor of a Battery pack object
 *
 * Creates and initializes a battery pack.
 * @param void
 * @return void
 */
cBattery::cBattery()
{
	count = 0;
	Vout = 0;
	Iout = 0;
	ElapsedTime = 0;
	CutOffVoltage = 7;
<<<<<<< HEAD
	tollarance = 0.005; //50mV
=======
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69
	SimulatorState.unlock();
	for(int i=0; i<3; i++)
		Switch[i] = false;
}

/**
 * @brief Returns the switch state of a particular cell
 *
 * @param cell The cell number
 * @return true If the switch is on
 * @return false If the switch is off
 * @see Switch
 */
bool cBattery::getSwitchState(int cell)
{
	bool result;
	AccessSynchroniser.lock();
	result = Switch[cell];
	AccessSynchroniser.unlock();
	return result;
}

/**
 * @brief Returns the time in milisec for which the battery has ran
 *
 * The Elapsed time accounts for how long it has been running.
 * The Elapsed time resets to 0 when the battery is resetted.
 *
 * @param void
 * @return elapsed time in milisecond
 * @see ElapsedTime
 */
double cBattery::getElapsedTime(void)
{
	double result;
	AccessSynchroniser.lock();
	result = ElapsedTime;
	AccessSynchroniser.unlock();
	return result;
}

/**
 * @brief Resets the battery and its cells
 *
 * Resets the battery to initial state.
 * Resets the connected cells. Resets Elapsed time to 0.
 *
 * @param void
 * @return true  successsfully reseted the battery.
 * @return false failed to reset one or more connected cell.
 */
bool cBattery::reset(void)
{
	bool status = false;
	bool lockStatus = false;
	for(int i=0; i<count; i++)
	{
		lockStatus = Cell[i]->lock(this);
		status = Cell[i]->loadDefaults(this);
		if(!status)
			break;
		if(lockStatus)
			Cell[i]->unlock(this);
	}
	ElapsedTime = 0;
	Vout = 0;
	Iout = 0;
	return status;
}

/**
 * @brief Runs the battery with a load
 *
 * Repeteadly calculate the battery parameters with
 * a load in a fixed interval.
 *
 * @param double load 		Load to be connected with
 * @param double resolution	The interval between two successive calculatein, in miliseconds.
 * @param double speed		Speed of the calculation. reduces the wait time between two calculations.
 * @return true successfully started to run the battery
 * @return false battery is already runing
 */
bool cBattery::run(double load,double resolution,double speed)
{
	if(IsRunning())
		return false;
	SimulatorState.lock();
	Runner = new std::thread(&cBattery::runBattery, this, load, resolution, speed);
	return true;
}

/**
 * @brief Stops a battery if it is running
 *
 * Signals the runner thread to stop and wait for it to stop.
 * @param void
 * @return true successsfully stopped the battery
 * @return false battery wasnot running
 */
bool cBattery::stop(void)
{
	if(IsRunning())
	{
		SimulatorState.unlock();
		Runner->join();
		return true;
	}
	return false;
}

/**
 * @brief Adds a cell to the battery
 *
 * Addes a cell to the battery if it is not ruuning and not full.
 *
 * @param cCell* Adcell Pointer to a cell object
 * @return true successfully added the cell
 * @return false battery is running or the battery is full
 * @see cCell
 */
bool cBattery::addCell(cCell* AdCell)
{
	if(IsRunning())
		return false;
	if(count>=3)
		return false;
	Cell[count++] = AdCell;
	return true;
}

/**
 * @brief Sets the cutoff voltage of the battery
 *
 * Sets the cutoff voltage. If the battery output voltage
 * goes down below this, the battery will be stopped
 *
 * @param double cutoff the cutoff voltage to be set
 * @return true successfully set the cutoff voltage
 * @return false battery is running
 */
bool cBattery::setCutOffVoltage(double cutoff)
{
	if(IsRunning())
		return false;
	AccessSynchroniser.lock();
	CutOffVoltage = cutoff;
	AccessSynchroniser.unlock();
	return true;
}

/**
 * @brief returns the Battery output voltage
 *
 * @param void
 * @return the output voltage in volt
 */
double cBattery::getVout(void)
{
	double result;
	AccessSynchroniser.lock();
	result = Vout;
	AccessSynchroniser.unlock();
	return result;
}

/**
 * @brief returns the Battery output current
 *
 * @param void
 * @return the output current in mini Ampere (mA)
 */
double cBattery::getIout(void)
{
	double result;
	AccessSynchroniser.lock();
	result = Iout;
	AccessSynchroniser.unlock();
	return (result*1000);
}

/**
 * @brief Determines the runner thread is still runnig or not
 *
 * wrapper function to ContinueRunning()
 * @param void
 * @return true Battery is running
 * @return false Battery is not running
 * @see ContinueRunnig
 */
bool cBattery::IsRunning(void)
{
	return ContinueRunning();
}

/**
 * @brief Determines wheather the simulator state is locked or not.
 *
 * Determines if the simulator state is locked. The runner thread will queary
 * and continue to run if simulator state is locked.
 * Simulator state also states whether the thread is runnig or not.
 * @param void
 * @return true Simulator state is locked
 * @return false simulator state is unlocked
 * @see IsRunning
 * @see SimulatorState
 */
bool cBattery::ContinueRunning(void)
{
	if(SimulatorState.try_lock())
	{
		SimulatorState.unlock();
		return false;
	}
	return true;
}

/**
 * @brief The runner thread function that updates the battery parameters
 *
 * Runs untill a stop signal is received or battery voltage goes down cutoff voltage
 * in a specific speed and update the battery parameters and cells in a specific interval.
 * writes to a  log file for each run.
 *
 * @param double load 		Load to be connected with
 * @param double resolution	The interval between two successive calculatein, in miliseconds.
 * @param double speed		Speed of the calculation. reduces the wait time between two calculations.
 * @return void
 */
void cBattery::runBattery(double load, double resolution, double speed)
{
	if(resolution == 0 || speed == 0 || load == 0)
		return;
	bool status = false;
	int i;
<<<<<<< HEAD
	double seriesResistance[count];
=======
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69
	for(i=0; i<count; i++)
	{
		status = Cell[i]->lock(this);
		if(!status)
			return;
<<<<<<< HEAD
		seriesResistance[i] = Cell[i]->getSeriesResistance();
	}

	int j, iTemp;
	double dTemp;
	double outVolt=0;
	bool localSwitch[count];
	int sortedCells[count];
	double cellVoltages[count];
	double tempVoltages[count];
	double sourceCurrent[count];
	double ratio;
	
	FILE* logFile;
	logFile = fopen("./batsim.log","a");

=======
	}

	double OutVolt=0;
	double divisor = 0;
	bool change = false;
	bool localSwitch[3] = {false,false,false};

	FILE* logFile;

	logFile = fopen("./batsim.log","a");


>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69
	fprintf(logFile,"***************************************************\n");
	fprintf(logFile,"\t\t\tBattery Simulator\n");
	fprintf(logFile,"***************************************************\n");
	fprintf(logFile,"\n[%9.3f]\tSimulator Started\n",ElapsedTime/1000);

	while(ContinueRunning())
	{

<<<<<<< HEAD
		fprintf(logFile,"\n[%9.3f]\toutVolt: %f\tIout: %f\n\tCell 1:: %d: %f V,\t%f mA\n\tCell 2:: %d: %f V,\t%f mA\n\tCell 3:: %d: %f V,\t%f mA\n",
			ElapsedTime/1000,Vout,Iout*1000,Switch[0],Cell[0]->getCurrentVoltage(),Cell[0]->getSourceCurrent()*1000,Switch[1],Cell[1]->getCurrentVoltage(),Cell[1]->getSourceCurrent()*1000,Switch[2],Cell[2]->getCurrentVoltage(),Cell[2]->getSourceCurrent()*1000);

	for(i=0;i<count;i++)
	{
		localSwitch[i] = false;
		sortedCells[i] = i;
		cellVoltages[i] = Cell[i]->getCurrentVoltage();
		tempVoltages[i] = cellVoltages[i];
	}
		
	for(i=0;i<count;i++)
	{
		for(j=i+1;j<count;j++)
		{
			if(tempVoltages[i]<tempVoltages[j])
			{
				iTemp=sortedCells[i];
				dTemp=tempVoltages[i];
				sortedCells[i]=sortedCells[j];
				tempVoltages[i]=tempVoltages[j];
				sortedCells[j]=iTemp;
				tempVoltages[j]=dTemp;
			}
		}
	}
	
	localSwitch[sortedCells[0]] = true;
	outVolt = cellVoltages[sortedCells[0]];
	for(i=1;i<count;i++)
	{
		if((cellVoltages[sortedCells[0]] - cellVoltages[sortedCells[i]])<=tollarance)
		{
			localSwitch[sortedCells[i]] = true;
			outVolt = cellVoltages[sortedCells[i]];
		}
	}
	ratio = 0;
	for(i=0;i<count;i++)
	{
		if(localSwitch[i])
			ratio += cellVoltages[i]/seriesResistance[i];
	}
	
	AccessSynchroniser.lock();
	Vout = outVolt;
	Iout = Vout / load;
	
	for(i=0;i<count;i++)
	{
		if(localSwitch[i])
			sourceCurrent[i] = (Iout*cellVoltages[i])/(ratio * seriesResistance[i]);
		else
			sourceCurrent[i] = 0;
		Switch[i] = localSwitch[i];
		Cell[i]->update(this,localSwitch[i],sourceCurrent[i],resolution);
	}
	AccessSynchroniser.unlock();
=======
		fprintf(logFile,"\n[%9.3f]\tOutVolt: %f\tIout: %f\n\tCell 1:: %d: %f V,\t%f mA\n\tCell 2:: %d: %f V,\t%f mA\n\tCell 3:: %d: %f V,\t%f mA\n",
			ElapsedTime/1000,Vout,Iout*1000,Switch[0],Cell[0]->getCurrentVoltage(),Cell[0]->getSourceCurrent()*1000,Switch[1],Cell[1]->getCurrentVoltage(),Cell[1]->getSourceCurrent()*1000,Switch[2],Cell[2]->getCurrentVoltage(),Cell[2]->getSourceCurrent()*1000);

		for(i=0;i<count;i++)
			localSwitch[i] = true;
		do
		{
			change = false;
			OutVolt = 0;
			divisor =0;
			for (i=0;i<count;i++)
			{
				if(localSwitch[i])
				{
					OutVolt += ((Cell[i]->getCurrentVoltage())/Cell[i]->getSeriesResistance());
					divisor += (1/Cell[i]->getSeriesResistance());
				}
			}
			divisor += (1/load);
			OutVolt = OutVolt/divisor;
			for(i=0;i<count;i++)
			{
				if(localSwitch[i] && (Cell[i]->getCurrentVoltage() < OutVolt))
				{
					localSwitch[i] = false;
					change = true;
				}
			}
		}while(change);
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69

		//sleep for Inteval
		usleep(resolution*1000/speed);
		AccessSynchroniser.lock();
		ElapsedTime += resolution;
		AccessSynchroniser.unlock();

		//if total voltage < MIN, break;
<<<<<<< HEAD
		if(outVolt < CutOffVoltage)
=======
		if(OutVolt < CutOffVoltage)
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69
		{
			for(i = 0; i<count; i++)
				localSwitch[i] = false;
			SimulatorState.unlock();
			std::cout<<"\nBattery exhausted\nSimulation completed\n";
			fprintf(logFile,"ALERT :: exhausted\n");
			std::cout<<"BatSim >> ";
		}
<<<<<<< HEAD
=======
		AccessSynchroniser.lock();
		//update out voltage
		Vout = OutVolt;

		//update the cells, switches and out current
		Iout = 0;
		for(i = 0; i<count; i++)
		{
			Cell[i]->update(this,localSwitch[i],Vout,resolution);
			//update total current
			Iout+= Cell[i]->getSourceCurrent();
			//update the switch
			Switch[i] = localSwitch[i];
		}
		AccessSynchroniser.unlock();
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69
	}
	fprintf(logFile,"\n[%9.3f]\tSimulator Stopped\n",ElapsedTime/1000);
	fcloseall();
	for(i =0;i<count;i++)
		Cell[0]->unlock(this);
	return;
}
