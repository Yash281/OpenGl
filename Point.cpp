#include "Point.h"



Point::Point(int x,int y)
{
	this->x = x;
	this->y = y;
}

int Point::getx()
{
	return x;
}

int Point::gety()
{
	return y;
}

void Point::setx(int posx)
{
	this->x = posx;
}

void Point::sety(int posy)
{
	this->y = posy;
}

Point::~Point()
{
}
