/**
 * @file cell.hpp
 * @brief Defines a cell class.
 *
 * Cell is the primary element of a battery.
 * A battery contains multiple cells connected in parallel.
 * Characteristics of a cell like discharge curve, capacity
 * etc decides the characteristics of the battery.
 * A cell should be added to a battarey for operation.
 *
 * @date Dec 1, 2014
 * @author Dipta Pandit
 **/
#ifndef  CELL_CLASS
#define  CELL_CLASS

//forword declaration of Battery class
class cBattery;

/**
 * @brief defines a cell
 *
 * Set initial voltage, series resistance, and characteristic parameters
 * before adding to a battery. When added  to a battery the cell should be
 * locked.
 **/
class cCell
{
	public:
		cCell();
		bool setInitialVoltage(double);
		bool setSeriesResistance(double);
		bool setCapacity(double);
		bool setShiftingPoints(double, double);
		bool setDropAmounts(double, double);
		double getInitialVoltage(void);
		double getSeriesResistance(void);
		double getSourceCurrent(void);
		double getCurrentVoltage(void);
		double getCapacity(void);
		double getRemainingCapacityPercentage(void);
		bool lock(cBattery*);
		bool unlock(cBattery*);
		bool update(double, double);
		bool loadDefaults(cBattery*);
	private:
		bool Locked;				///<Denotes the cell is connected to a battery and the parameters are locked
		cBattery* AttachedTo;		///<Denotes which battery it is connected to
		double InitialVoltage;		///<Initial voltage of the cell inn Volts. @see setInitialVoltage @see getInitialVoltage
		double SeriesResistance;	///<Series resistance of the cell in Ohms. @see setSeriesResistance @see getSeriesResistance
		double Capacity;			///<Initial capacity of the cell in AmS. @see setCapacity @see getCapacity
		double Shift1;				///<First gradient change in the discharge curve. expressed as percentage of descharged capacity.
		double Shift2;				///<Second gradient change in the discharge curve. expressed as percentage of descharged capacity.
		double Drop1;				///<Voltage drop at shift1. expressed as percentage
		double Drop2;				///<Voltage drop at shift2. expressed as percentage
		double Gradient;			///<Current slop of the discharge curve. Measured as Voltage per Capacity
		double ConstantK;			///<Constant factor of discharge curve equation. Measeured as Volt.
		double m1,m2,m3;			///<One of three relative gradients. Constant ratio.
		double DischargedCapacity;  ///<Capacity of the cell already discharged. Unit AmS.
		double RemainigCapacity;	///<Percentage of capacity remaining. (%)
		double SourceCurrent;		///<Current sourced by the cell in Ampere.
		double CurrentVoltage;		///<Current voltage of the cell in Volts.
		void initialise(void);
};

#endif //CELL_CLASS
