#include "stdafx.h"
#include "Blocks.h"


Blocks::Blocks(int x, int y, int width, int color)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->color = color;
	this->enabled = true;
}


Blocks::~Blocks()
{
}

void Blocks::Collide(Graphic item) {
	this->enabled = false;
}
