#pragma once
#include "Graphic.h"

class Blocks :
	public Graphic
{
public:
	Blocks(int x, int y,int width, int color);
	~Blocks();
	int width;
	void Collide(Graphic item) override;
};

