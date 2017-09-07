#pragma once
class Point
{
private:
	int x, y;
public:
	Point(int x,int y);
	int getx();
	int gety();
	void setx(int posx);
	void sety(int posy);
	~Point();
};

