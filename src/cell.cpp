/**
 * @file cell.cpp
 * @brief Implementation of the cell class
 *
 * Cell is the primary element of a battery.
 * A battery contains multiple cells connected in parallel.
 * Characteristics of a cell like discharge curve, capacity
 * etc decides the characteristics of the battery.
 * A cell should be added to a battarey for operation.
 * @author Dipta Pandit
 * @date 3/12/2014
 * @see cell.hpp
 */
#include "../includes/cell.hpp"

/**
 * @brief Constructor of a cell object
 *
 * Creates a cell object and initialize it with some default value
 * @param void
 * @return void
 */
cCell::cCell()
{
	Capacity = 800*3600;
	InitialVoltage = 12;
	SeriesResistance = 20;
	Shift1 = 10;
	Shift2 = 90;
	Drop1 = 20;
	Drop2 = 60;
	AttachedTo = (cBattery*)0;
	Locked = false;
}

/**
 * @brief Sets the initialvoltage of the cell
 *
 * Initial voltage is the voltage that the cell voltage
 * starts with when it is updated.
 * Cellvoltage will be set to this voltage when the cell is resetted.
 *
 * @param double initial voltage to set in Volts
 * @return bool true if successfully set the initial voltage
 * false if the cell is locked.
 */
bool cCell::setInitialVoltage(double initv)
{
	if(Locked)
		return false;
	InitialVoltage = initv;
	return true;
}

/**
 * @brief Sets the internal series resistance of the cell
 *
 * series resistance is the fixed resistance of the cell
 * that can be assumed is connected in series with the cell
 * when it is operatied
 *
 * @param double series resistance to be set in Ohms
 * @return bool true if successfully set the series resistance
 * false if the cell is locked.
 */
bool cCell::setSeriesResistance(double sres)
{
	if(Locked)
		return false;
	SeriesResistance = sres;
	return true;
}

/**
 * @brief Sets the capacity of the cell
 *
 * capacity is the ability of the cell torun for a
 * particular time when delivering a particulat current.
 * It is the product of amount of current it can deliver and for the time
 *
 * @param double cap capacity to set in mAH
 * @return bool true if successfully set the capacity
 * false if the cell is locked.
 */
bool cCell::setCapacity(double cap)
{
	if(Locked)
		return false;
	Capacity = cap * 3600;
	return true;
}

/**
 * @brief Sets the gradient shifting points
 *
 * Sets the gradient shifting points on the capacity axis
 * of the discharge curve.
 *
 * @param double 1st shifting point on the capacity axis as percentage
 * @param double 2nd shifting point on the capacity axis as percentage
 * @return bool true if successully sets the value
 * false if cell is locked or value is not in range
 */
bool cCell::setShiftingPoints(double sh1, double sh2)
{
	if(Locked)
		return false;
	if(sh2 <= sh1)
		return false;
	Shift1 = sh1;
	Shift2 = sh2;
	return true;
}

/**
 * @brief Sets the drop percentage of voltage at shifting points
 *
 * The drop percentage determines the discharge curve
 * @param double drop of voltage at shift point 1 as percentage
 * @param double drop of voltage at shift point 2 as percentage
 * @return bool true if successfully sets the value.
 * false if cell is locked or parameters are not in range.
 */
bool cCell::setDropAmounts(double d1, double d2)
{
	if(Locked)
		return false;
	if(d1>100 ||d1<0 ||d2 >100 ||d2<0)
		return false;
	if(d2 <= d1)
		return false;
	Drop1 = d1;
	Drop2 = d2;
	return true;
}
/**
 * @brief locks the cell to a battery
 *
 * @param cBattery Pointer to the battery who wants the lock
 * @return bool true if successfully locked.
 * false if it is already locked
 */
bool cCell::lock(cBattery* owner)
{
	if(Locked)
		return false;
	AttachedTo = owner;
	Locked = true;
	initialise();
	return true;
}
/**
 * @brief unlocks the cell from its battery
 *
 * @param cBattery Owner of the cell
 * @return bool true if successfully unlocked.
 * false if owner does not own the battery or it is not locked
 */
bool cCell::unlock(cBattery* owner)
{
	if(!Locked)
		return false;
	if(AttachedTo != owner)
		return false;
	AttachedTo = (cBattery*)0;
	Locked = false;
	return true;
}

/**
 * @brief Returns the initial voltage of the cell
 *
 * @param void
 * @return double voltage in Volts
 */
double cCell::getInitialVoltage(void)
{
	double result;
	AccessSynchroniser.lock();
	result = InitialVoltage;
	AccessSynchroniser.unlock();
	return result;
}

/**
 * @brief Returns the current voltage level of the cell
 *
 * Curent voltage denotes the voltagelevel the cell is
 * having at any particular time. It si set to initial
 * voltage when the cell is resetted
 *
 * @param void
 * @return double current voltage in Volts
 */
double cCell::getCurrentVoltage(void)
{
	double result;
	AccessSynchroniser.lock();
	result = CurrentVoltage;
	AccessSynchroniser.unlock();
	return result;
}

/**
 * @brief Returns the series resistance of the cell
 *
 * The series resistance is a constant resistance known
 * as internal series resistance or equvalant series resistance
 * @param void
 * @return double resistance in Ohms
 */
double cCell::getSeriesResistance(void)
{
	double result;
	AccessSynchroniser.lock();
	result = SeriesResistance;
	AccessSynchroniser.unlock();
	return result;
}

/**
 * @brief Returns the current being sourced by the cell
 *
 * The current current being sourced by the cell is returned in Ampere
 * @param void
 * @return double current in A
 */
double cCell::getSourceCurrent(void)
{
	double result;
	AccessSynchroniser.lock();
	result = SourceCurrent;
	AccessSynchroniser.unlock();
	return result;
}

/**
 * @brief Returns the capacity of the cell
 *
 * @param void
 * @return double capacity of the cell in mAH
 */
double cCell::getCapacity(void)
{
	double result;
	AccessSynchroniser.lock();
	result = Capacity/3600;
	AccessSynchroniser.unlock();
	return result;
}

/**
 * @brief Returns the remaining capacity of the cell as percentage
 *
 * @param void
 * @return double remaining capacity as percentage.
 */
double cCell::getRemainingCapacityPercentage(void)
{
	return RemainigCapacity;
}

/**
 * @brief Initializes the battery characteristics parameters
 *
 * Sets the discharge curve characteristics and gradients
 * @param void
 * @return void
 */
void cCell::initialise(void)
{
	AccessSynchroniser.lock();
	m1 = (InitialVoltage * Drop1) / (Capacity * Shift1);
	m2 = (InitialVoltage * (Drop2 - Drop1)) / (Capacity * (Shift2 - Shift1));
	m3 = (InitialVoltage * (100 - Drop2)) / (Capacity * (100 - Shift2));

	CurrentVoltage = InitialVoltage;
	DischargedCapacity = 0;
	RemainigCapacity = 100;
	Gradient = m1;
	ConstantK = 0;
	AccessSynchroniser.unlock();
	return;
}

/**
 * @brief Updates the cell parameters
 *
 * Updates the cell parameters as if it is connected to a output
 * voltage for specific time. This actually implements the three
 * stage liniar approximation of the discharge curve of the cell
 *
 * @param cBattery* owner Owner of te cell
 * @param bool connected Weather or not the cell is connected
 * @param double vout 	 The output voltage it is connected to in Volt
 * @param double runtime For how long it was connected in milisec
 * @return bool true if Successfully update the cell.false if
 * The cell is not owned by the battery or runtime is 0
 */
bool cCell::update(cBattery* owner,bool connected, double vout, double runtime)
{
	if(AttachedTo != owner)
		return false;
	if(0 == runtime)
		return false;
	if(!connected)
	{
		SourceCurrent = 0;
		return true;
	}
	AccessSynchroniser.lock();
	SourceCurrent = (CurrentVoltage - vout) / SeriesResistance;
	DischargedCapacity += (SourceCurrent * runtime);
	RemainigCapacity = ((Capacity - DischargedCapacity) / Capacity) * 100;
	CurrentVoltage = CurrentVoltage - Gradient*SourceCurrent*runtime + ConstantK;

	if((Gradient == m1) && (DischargedCapacity >= ((Shift1*Capacity)/100)))
	{
		Gradient = m2;
		ConstantK += (m2 - m1)*DischargedCapacity;
	}
	if((Gradient == m2) && (DischargedCapacity >= ((Shift2*Capacity)/100)))
	{
		Gradient = m3;
		ConstantK += (m3 - m2)*DischargedCapacity;
	}
	AccessSynchroniser.unlock();
	return true;
}

/**
 * @brief Loads the default values of the cell
 *
 * Resets the cell voltage to initial voltage
 * and, capacity to its full.
 *
 * @param cBattery* owner POinter to the Battery pack that the caell is attached to
 * @return true if successfuly loaded the default values.false if The battrey does not own the cell.
 */
bool cCell::loadDefaults(cBattery* owner)
{
	if(owner != AttachedTo)
		return false;
	AccessSynchroniser.lock();
	CurrentVoltage = InitialVoltage;
	RemainigCapacity = 100;
	DischargedCapacity = 0;
	Gradient = m1;
	ConstantK = 0;
	AccessSynchroniser.unlock();
	return true;
}
