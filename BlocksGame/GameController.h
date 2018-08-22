#pragma once
#include "Blocks.h"
#include "Square.h"

class GameController
{
public:
	GameController(int startCol, int startRow, int gameWidth, int gameHeight);
	~GameController();
	int boardSpeed = 2;
	int startCol;
	int startRow;
	int startTxtCol = 0;
	int startTxtRow = -2;
	int gameWidth;
	int gameHeight;
	bool end = true;
	int boardWidth = 5;
	int blocksNum = 0;
	bool** map;
	Blocks*** blocksMap;
	Blocks* board;
	Square* square;

	void BoardGoRight();
	void BoardGoLeft();
	void Initialize();
	void Render();
	void Update();
	void CheckCollide();
	void gotoxy(int xpos, int ypos);
	void Lose();
	void Victory();
	void ShowWord(char* set);
	void Erase(Blocks blocks);
	void Erase(Square square);
};

