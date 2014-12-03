/**
 * @file battery.hpp
 * @brief Defines a battery class.
 *
 * A battery is consist of one or more cells and switches.
 * Battery provides a output voltage and output current when
 * connected to a load. It discharges the cells untill
 * a cutoff voltage is reached.
 *
 * @see cell.hpp
 *
 * @date Dec 1, 2014
 * @author Dipta Pandit
 **/
#ifndef  BATTERY_CLASS
#define  BATTERY_CLASS

#include "cell.hpp"
#include <thread>
#include <mutex>


/**
 * @brief defines a battery
 *
 * Add cells to the battery and run for a specific load.
 * The cells will be updated in specific interval at a specific speed.
 * The connected cells can be resetted from its battery.
 *
 * @see cCell
 **/
class cBattery
{
	public:
		cBattery();
		bool reset(void);
		bool run(double load,double resolution,double speed);
		bool stop(void);
		bool addCell(cCell* AdCell);
		double getVout(void);
		double getIout(void);
		bool setCutOffVoltage(double cutoff);
		double getElapsedTime(void);
		bool getSwitchState(int Cell);
		bool IsRunning(void);
	private:
		cCell *Cell[3];		///<Holds the cells that are added. @see addCell
		bool Switch[3];		///<A switch for each cell
		double Vout;			///<Output voltage of the battery in Volts.
		double Iout;			///<Output current of the battery in Ampere.
		double ElapsedTime;		///<Time for which the battery is running in mS.
		double CutOffVoltage;	///<Battery will be disconnected when Output voltage drops below this. expressed in Volts.
		std::thread* Runner;	///<Pointer to the runner thread
		std::mutex SimulatorState;	///<Used to signal thread terminaton event
		void runBattery(double load,double resolution,double speed);
		bool ContinueRunning(void);
		int count;				///<number of cells added in the battery
		std::mutex AccessSynchroniser; ///<Lock to synchronize access to members from different thread
};

#endif //BATTERY_CLASS
