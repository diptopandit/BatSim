#include "../includes/cell.hpp"

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

bool cCell::setInitialVoltage(double initv)
{
	if(Locked)
		return false;
	InitialVoltage = initv;
	return true;
}

bool cCell::setSeriesResistance(double sres)
{
	if(Locked)
		return false;
	SeriesResistance = sres;
	return true;
}

bool cCell::setCapacity(double cap)
{
	if(Locked)
		return false;
	Capacity = cap * 3600;
	return true;
}

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

bool cCell::lock(cBattery* owner)
{
	if(Locked)
		return false;
	AttachedTo = owner;
	Locked = true;
	initialise();
	return true;
}

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

double cCell::getInitialVoltage(void)
{
	return InitialVoltage;
}

double cCell::getCurrentVoltage(void)
{
	return CurrentVoltage;
}

double cCell::getSeriesResistance(void)
{
	return SeriesResistance;
}

double cCell::getSourceCurrent(void)
{
	return SourceCurrent;
}

double cCell::getCapacity(void)
{
	return Capacity;
}

double cCell::getRemainingCapacityPercentage(void)
{
	return RemainigCapacity;
}

void cCell::initialise(void)
{
	m1 = (InitialVoltage * Drop1) / (Capacity * Shift1);
	m2 = (InitialVoltage * (Drop2 - Drop1)) / (Capacity * (Shift2 - Shift1));
	m3 = (InitialVoltage * (100 - Drop2)) / (Capacity * (100 - Shift2));

	CurrentVoltage = InitialVoltage;
	DischargedCapacity = 0;
	RemainigCapacity = 100;
	Gradient = m1;
	ConstantK = 0;
	return;
}

bool cCell::update(double outVolt, double milisec)
{
	if(0 == milisec)
		return false;
	SourceCurrent = (CurrentVoltage - outVolt) / SeriesResistance;
	DischargedCapacity += (SourceCurrent * milisec);
	RemainigCapacity = ((Capacity - DischargedCapacity) / Capacity) * 100;
	CurrentVoltage = CurrentVoltage - Gradient*SourceCurrent*milisec + ConstantK;
	
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
	return true;
}

bool cCell::loadDefaults(cBattery* owner)
{
	if(owner != AttachedTo)
		return false;
	CurrentVoltage = InitialVoltage;
	RemainigCapacity = 100;
	DischargedCapacity = 0;
	Gradient = m1;
	ConstantK = 0;
	return true;
}




