
/**
 * @file simulator.c
 * @brief Battery simulator.
 *
 * Implements the command line interface for the simulator.
 * @author Dipta Pandit
 * @date 30 Nov 2014
 * @see batsim.hpp
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
<<<<<<< HEAD
=======
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
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69
	return;
}

int main()
{
	cCell Cell[3];
	cBattery BatPack;
	cBatSim Simulator;

<<<<<<< HEAD
	Cell[0].setInitialVoltage(10.2);
	Cell[1].setInitialVoltage(10.6);
	Cell[2].setInitialVoltage(10.4);

	//Cell[0].setSeriesResistance(50);
	//Cell[1].setSeriesResistance(30);
	//Cell[2].setSeriesResistance(40);
=======
	Cell[0].setInitialVoltage(12.2);
	Cell[1].setInitialVoltage(10.9);
	Cell[2].setInitialVoltage(11.8);

	Cell[0].setSeriesResistance(30);
	Cell[1].setSeriesResistance(10);
	Cell[2].setSeriesResistance(20);
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69

	BatPack.addCell(&Cell[0]);
	BatPack.addCell(&Cell[1]);
	BatPack.addCell(&Cell[2]);

	Simulator.connect(&BatPack);
	Simulator.connect(160);
	Simulator.setSpeed(1000);
<<<<<<< HEAD
	Simulator.setResolution(10);
=======
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69

	const char* validCommands[] = {"get","set","setadv","sim","help","exit",(char*)0};
	const char* validKeys[] = {"cvoltage","load","scurrent","vout","iout","status","runtime","sresistance","start","stop","shift","drop","cutoff","capacity","initv","sstate",(char*)0};
	cParser Parser;
	bool exit_loop = false;
	double runtime;
	int hh, mm, ss, uuu;
	int Function = 0;
	int i;

	while(!exit_loop)
	{
		if(Parser.getInput())
		{
			if(Parser.parseInput(validCommands,validKeys))
			{
				runtime = BatPack.getElapsedTime();
				hh = (int)(runtime/(1000*3600));
				runtime -= hh*3600*1000;
				mm = (int)(runtime/(1000*60));
				runtime -= mm*60*1000;
				ss = (int)(runtime/1000);
				runtime -= ss*1000;
				uuu = (int)(runtime);
				std::cout<<"["<<std::setw(2)<<std::setfill('0')<<hh<<":"<<mm<<":"<<ss<<":"<<std::setw(3)<<uuu<<"]\n";
				Function = Parser.getFunctionNumber();
				switch(Function)
				{
					case GETCVOL:
						if(Parser.getParamCount() > 0)
							std::cout<<"Extra parameters omitted."<<std::endl;
						std::cout<<"Cell Voltage:\n";
						for(i =0; i<3 ; i++)
							std::cout<<"Cell "<<i<<": "<<std::fixed<<std::setprecision(3)<<Cell[i].getCurrentVoltage()<<" V.\n";
					break;
					case GETLOAD:
<<<<<<< HEAD
						std::cout<<"Connected Load: "<<Simulator.getLoad()<<" Ohm."<<std::endl;
=======
						std::cout<<"Connected Load: "<<Simulator.getLoad()<<" V."<<std::endl;
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69
					break;
					case GETSCUR:
						if(Parser.getParamCount() > 0)
							std::cout<<"Extra parameters omitted."<<std::endl;
						std::cout<<"Source current:\n";
						for(i =0; i<3 ; i++)
							std::cout<<"Cell "<<i<<": "<<std::fixed<<std::setprecision(3)<<Cell[i].getSourceCurrent()*1000<<" mA.\n";
					break;
					case GETVOUT:
						std::cout<<"Output Voltage: "<<std::setprecision(3)<<BatPack.getVout()<<" V."<<std::endl;
					break;
					case GETIOUT:
						std::cout<<"Output Current: "<<std::fixed<<std::setprecision(3)<<BatPack.getIout()<<" mA."<<std::endl;
					break;
					case GETSTAT:
<<<<<<< HEAD
						std::cout<<"-----------------------------------------------------------------\n";
						std::cout<<"Connected Load : "<<Simulator.getLoad()<<" Ohm."<<std::endl;
						std::cout<<"-----------------------------------------------------------------\n";
						std::cout<<"Parameter\t\tCell1\t\tCell2\t\tCell3\n";
						std::cout<<"-----------------------------------------------------------------\n";
						std::cout<<"Voltage\t\t\t"<<std::fixed<<std::setprecision(3)<<Cell[0].getCurrentVoltage()<<"\t\t"<<Cell[1].getCurrentVoltage()<<"\t\t"<<Cell[2].getCurrentVoltage()<<std::endl;
						std::cout<<"SourceCurrent\t\t"<<std::fixed<<std::setprecision(3)<<Cell[0].getSourceCurrent()*1000<<"\t\t"<<Cell[1].getSourceCurrent()*1000<<"\t\t"<<Cell[2].getSourceCurrent()*1000<<std::endl;
						std::cout<<"Switch state\t\t"<<BatPack.getSwitchState(0)<<"\t\t"<<BatPack.getSwitchState(1)<<"\t\t"<<BatPack.getSwitchState(2)<<std::endl;
						std::cout<<"==================================================================\n";
						std::cout<<"Output Voltage: "<<std::setprecision(3)<<BatPack.getVout()<<" V."<<std::endl;
						std::cout<<"Output Current: "<<std::fixed<<std::setprecision(3)<<BatPack.getIout()<<" mA."<<std::endl;
						std::cout<<"==================================================================\n\n\n";
					break;
					case GETRUNT:
						std::cout<<"Runtime: "<<BatPack.getElapsedTime()<<" ms"<<std::endl;
=======
					break;
					case GETRUNT:
						std::cout<<"Runtime: "<<BatPack.getElapsedTime()<<std::endl;
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69
					break;
					case GETSWCH:
						if(Parser.getParamCount() > 0)
							std::cout<<"Extra parameters omitted."<<std::endl;
						std::cout<<"Switch states:\n";
						for(i =0; i<3 ; i++)
						{
							std::cout<<"Switch "<<i<<": ";
							if(BatPack.getSwitchState(i))
							std::cout<<"ON\n";
						else
							std::cout<<"OFF\n";
						}
					break;
					case SETINTV:
						if(Parser.getParamCount() < 1)
						{
							std::cout<<"Insufficient arguments. Specify one or more cell's initial voltage."<<std::endl;
							break;
						}
						std::cout<<"Setting...\n";
						for( i=0;i<Parser.getParamCount() && i<3;i++)
						{
							if(Cell[i].setInitialVoltage(Parser.getParam(i)))
								std::cout<<i+1<<": Done."<<std::endl;
							else
								std::cout<<i+1<<": Failed."<<std::endl;
						}
						if(Parser.getParamCount() > 3)
							std::cout<<"Extra parameters omitted."<<std::endl;
					break;
					case SETLOAD:
						if(Parser.getParamCount() < 1)
						{
							std::cout<<"Insufficient arguments. Specify load resistance."<<std::endl;
							break;
						}
						if(Simulator.connect(Parser.getParam(0)))
							std::cout<<"Connected\n";
						else
							std::cout<<"Error\n";
						if(Parser.getParamCount() > 1)
							std::cout<<"Extra parameters omitted."<<std::endl;
					break;
					case SETSRES:
						if(Parser.getParamCount() < 1)
						{
							std::cout<<"Insufficient arguments. Specify one or more cell's series resistance."<<std::endl;
							break;
						}
						std::cout<<"Setting...\n";
						for( i=0;i<Parser.getParamCount() && i<3;i++)
						{
							if(Cell[i].setSeriesResistance(Parser.getParam(i)))
								std::cout<<i+1<<": Done."<<std::endl;
							else
								std::cout<<i+1<<": Failed."<<std::endl;
						}
						if(Parser.getParamCount() > 3)
							std::cout<<"Extra parameters omitted."<<std::endl;
					break;
					case SETADVSH:
						if(Parser.getParamCount() < 3)
						{
							std::cout<<"Insufficient arguments. Specify cell number and 2 shift points."<<std::endl;
							break;
						}
						if(Parser.getParam(0)<1 || Parser.getParam(0)>3)
						{
							std::cout<<"Invalid cell number."<<std::endl;
							break;
						}
						std::cout<<"Setting...\n";
						if(Cell[(int)(Parser.getParam(0)-1)].setShiftingPoints(Parser.getParam(1),Parser.getParam(2)))
							std::cout<<"Done\n";
						else
							std::cout<<"Failed\n";
					break;
					case SETADVDP:
						if(Parser.getParamCount() < 3)
						{
							std::cout<<"Insufficient arguments. Specify cell number and 2 drop amounts."<<std::endl;
							break;
						}
						if(Parser.getParam(0)<1 || Parser.getParam(0)>3)
						{
							std::cout<<"Invalid cell number."<<std::endl;
							break;
						}
						std::cout<<"Setting...\n";
						if(Cell[(int)(Parser.getParam(0)-1)].setDropAmounts(Parser.getParam(1),Parser.getParam(2)))
							std::cout<<"Done\n";
						else
							std::cout<<"Failed\n";
					break;
					case SETADVCUT:
						if(Parser.getParamCount() < 1)
						{
							std::cout<<"Insufficient arguments. specify cutoff voltage for the battery."<<std::endl;
							break;
						}
						std::cout<<"Setting...\n";
						if(BatPack.setCutOffVoltage(Parser.getParam(0)))
							std::cout<<"Done\n";
						else
							std::cout<<"Failed\n";
					break;
					case SETADVCAP:
						if(Parser.getParamCount() < 1)
						{
							std::cout<<"Insufficient arguments. Specify one or more cell's capacity."<<std::endl;
							break;
						}
						std::cout<<"Setting...\n";
						for( i=0;i<Parser.getParamCount() && i<3;i++)
						{
							if(Cell[i].setCapacity(Parser.getParam(i)))
								std::cout<<i+1<<": Done."<<std::endl;
							else
								std::cout<<i+1<<": Failed."<<std::endl;
						}
						if(Parser.getParamCount() > 3)
							std::cout<<"Extra parameters omitted."<<std::endl;
					break;
					case SIMSTART:
						if(Parser.getParamCount() > 0)
							std::cout<<"Extra parameters omitted."<<std::endl;
						if(Simulator.start())
							std::cout<<"Simulation started."<<std::endl;
						else
							std::cout<<"Simulation already running."<<std::endl;
					break;
					case SIMSTOP:
						if(Parser.getParamCount() > 0)
							std::cout<<"Extra parameters omitted."<<std::endl;
						if(Simulator.stop())
							std::cout<<"Simulation stopped."<<std::endl;
						else
							std::cout<<"Simulation is not running currently."<<std::endl;
					break;
					case HELP:
						showHelp();
					break;
					case EXIT:
						Simulator.stop();
						std::cout<<"Exiting simulator now.\n";
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


