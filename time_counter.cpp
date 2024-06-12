#include "StdAfx.h"
#include "time_counter.h"

time_counter::time_counter(void)
{
}

time_counter::~time_counter(void)
{
}
void time_counter::settime()
{
	this->starting_time=time(NULL);
}
int time_counter::running_period()
{
	time_t temp= time(NULL);
	return (temp - this->starting_time);
}