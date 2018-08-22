#pragma once
class Graphic
{
public:
	Graphic(int x, int y);
	Graphic();
	~Graphic();
	int x;
	int y;
	bool enabled;
	int color;

	virtual void Collide(Graphic item);
};