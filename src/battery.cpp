#include "../includes/battery.hpp"

cBattery::cBattery(int number)
{
	Size = number;
	count = 0;
}

bool cBattery::reset(void)
{
	bool status;
	for(int i=0; i<Size; i++)
		status = Cell[i]->loadDefaults();
	return status;
}

bool run(double,double,double);
bool stop(void);

bool cBattery::addCell(cCell* AdCell)
{
	if(count>=Size)
		return false;
	Cell[count++] = AdCell;
	return true;
}
