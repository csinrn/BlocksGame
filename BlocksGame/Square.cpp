#include "stdafx.h"
#include "Square.h"


Square::Square(int x, int y, int color, int dir)
{
	this->x = x;
	this->y = y;
	this->color = color;
	this->dir = dir;
}


Square::~Square()
{
}

void Square::Collide(Graphic item)
{
	if (dir == 0) {  // when going upperLeft
		if (item.y < y) {
			dir = 3;   // if collide something higher, go lowerLeft
		}
		else {
			dir = 1;   // if collide something in same Level, go upperRight
		}
	}
	else if (dir == 1) {
		if (item.y < y) {
			dir = 2;
		}
		else {
			dir = 0;
		}
	}
	else if (dir == 2) {
		if (item.y > y) {
			dir = 1;
		}
		else {
			dir = 3;
		}
	}
	else if (dir == 3) {
		if (item.y > y) {
			dir = 0;
		}
		else {
			dir = 2;
		}
	}
}

