
/**
 * @file simulator.c
 * @brief Battery simulator.
 *
 * Implements the command line interface for the simulator.
 * @author Dipta Pandit
 * @date 30 Nov 2014
 * @see simulator.h
*/

#include "../includes/cell.hpp"
#include "../includes/battery.hpp"
#include "../includes/simulator.hpp"
#include "../includes/cparser.hpp"
#include "../includes/batsim.hpp"

#include <iostream>
#include <iomanip>


/**
 * @brief Shows the help text.
 *
 * @param void
 * @return void
 */
void showHelp(void)
{
	std::cout<<"\nBATSIM \n";
	std::cout<<"\nNAME\n\tbatsim - Batterypack simulator\n";
	std::cout<<"\nSYNOPSIS\n\tbatsim\n";
	std::cout<<"\nDESCRIPTION\n\tbatsim simulates a baterry pack with three parallel connected cells.\
			\n\tThe simulator will start a command line interface and accepts command to view and set various parameters.\
			\n\tGeneric command format is: BatSim>> <command> [<key> [<value1> [<value2> [<value3>]]]]\n";
	std::cout<<"\nCOMMANDS AND KEYWORDS\n\
			\n\tset   \tSets a value. Format: BatSim>> set <key> [<value1> [<value2> [<value3>]]]\
			\n\t      \tUnnecessary options/arguments are ignored. If required value is not provided, by default it takes 0.\
			\n\t      \tValid keys are: initv, sresistance, and load\
			\n\tget   \tReturns a parameter. Format: BatSim>> get <key>\
			\n\t      \tValid keys are: cvoltage, scurrent, vout, iout, load, runtime, sstate, and status\
			\n\tsetadv\tSets some advanced parameters. Format is same as set\
			\n\t      \tValid keys are: shift1, shift2, drop1, drop2, cutoff and capacity\
			\n\t      \tThese keys are advance battery characteristics.\
			\n\t      \tshift1, shift2: gradient shifting points on the capacity axis, expressed as percentage(%%).\
			\n\t      \tdrop1, drop2: percentage drop of voltage at shift1 and shift2 respectively(%%).\
			\n\tsim   \tStarts or stops the simulator. Format: BatSim>> sim start/stop\
			\n\t      \tSimulator log is generated ./batsim.log\
			\n\thelp  \tPrints this help text.\
			\n\texit  \tExits the simulator. If the simulator is still running, tries to stop it first.\n";
	std::cout<<"\nDEFAULT VALUES\n\
			\n\tInitial voltages  : 12.20 V, 10.90 V, 11.8 V\
			\n\tSeries resistances: 30 Ohm,  10 Ohm,  20 Ohm\
			\n\tLoad              : 160 Ohm\
			\n\tCapacity          : 800 mAH\
			\n\tshift1            : 10 %%\
			\n\tshift2            : 90 %%\
			\n\tdrop1             : 20 %%\
			\n\tdrop2             : 60 %%\
			\n\tCutoff voltage    : 07 V\n";
	return;
}


int main()
{
	cCell Cell[3];
	cBattery BatPack;
	cBatSim Simulator;

	Cell[0].setInitialVoltage(11.5);
	Cell[1].setInitialVoltage(11.9);
	Cell[2].setInitialVoltage(12.2);

	Cell[0].setSeriesResistance(20);
	Cell[1].setSeriesResistance(30);
	Cell[2].setSeriesResistance(15);

	BatPack.addCell(&Cell[0]);
	BatPack.addCell(&Cell[1]);
	BatPack.addCell(&Cell[2]);

	Simulator.connect(&BatPack);
	Simulator.connect(160);
	Simulator.setSpeed(1000);

	const char* validCommands[] = {"get","set","setadv","sim","help","exit",(char*)0};
	const char* validKeys[] = {"cvoltage","load","scurrent","vout","iout","status","runtime","sresistance","start","stop","shift1","shift2","drop1","drop2","cutoff","capacity","initv","sstate",(char*)0};
	cParser Parser;
	bool exit_loop = false;
	int Function = 0;

	while(!exit_loop)
	{
		if(Parser.getInput())
		{
			if(Parser.parseInput(validCommands,validKeys))
			{
				Function = Parser.getFunctionNumber();
				//std::cout<<"function: "<<Function<<std::endl;
				switch(Function)
				{
					case GETCVOL:
						if(Parser.getParamCount() < 1)
						{
							std::cout<<"Insufficient arguments. Specify cell number"<<std::endl;
							break;
						}
						if(Parser.getParam(0) > 3 || Parser.getParam(0) < 1)
						{
							std::cout<<"Invalid cell number"<<std::endl;
							break;
						}
						std::cout<<"Cell "<<Parser.getParam(0)<<" voltage: "<<Cell[(int)Parser.getParam(0)-1].getCurrentVoltage()<<" V"<<std::endl;
					break;
					case GETLOAD:
					break;
					case GETSCUR:
					break;
					case GETVOUT:
						std::cout<<"Output Voltage: "<<std::setprecision(3)<<BatPack.getVout()<<" V."<<std::endl;
					break;
					case GETIOUT:
						std::cout<<"Output Current: "<<std::fixed<<std::setprecision(3)<<BatPack.getIout()<<" mA."<<std::endl;
					break;
					case GETSTAT:
					break;
					case GETRUNT:
						std::cout<<"Runtime: "<<BatPack.getElapsedTime()<<std::endl;
					break;
					case GETSWCH:
						if(Parser.getParamCount() < 1)
						{
							std::cout<<"Insufficient arguments. Specify cell number"<<std::endl;
							break;
						}
						if(Parser.getParam(0) > 3 || Parser.getParam(0) < 1)
						{
							std::cout<<"Invalid cell number"<<std::endl;
							break;
						}
						std::cout<<"Switch"<<Parser.getParam(0)<<": ";
						if(BatPack.getSwitchState(Parser.getParam(0)-1))
							std::cout<<"ON";
						else
							std::cout<<"OFF";
						std::cout<<"."<<std::endl;
					break;
					case SIMSTART:
						if(Simulator.start())
							std::cout<<"Simulation started"<<std::endl;
						else
							std::cout<<"Simulation already running"<<std::endl;
					break;
					case SIMSTOP:
						if(Simulator.stop())
							std::cout<<"Simulation stopped"<<std::endl;
						else
							std::cout<<"Simulation is not running currently"<<std::endl;
					break;
					case HELP:
						showHelp();
					break;
					case EXIT:
						Simulator.stop();
						exit_loop = true;
					break;
					default:
						if(Function >=600)
							std::cout<<Parser.getLastCommand()<<" is not a valid Command."<<std::endl;
						else
							std::cout<<Parser.getLastKey()<<" is not a valid key for "<<Parser.getLastCommand()<<"."<<std::endl;
				}
			}
			else
				std::cout<<std::endl<<"Input correctly not recorded."<<std::endl;
		}
	}
	return 0;
}


