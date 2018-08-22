#pragma once
#include "Graphic.h"
class Square :
	public Graphic
{
public:
	Square(int x, int y, int color, int dir);
	~Square();
	int dir;  // 0 1 2 3 for upperLeft, upperRight,lowerRight, lowerLeft
	void Collide(Graphic item) override;
};

