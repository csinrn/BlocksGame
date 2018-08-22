#include "stdafx.h"
#include "GameController.h"
#include "Blocks.h"
#include "Windows.h"


GameController::GameController(int startCol, int startRow, int gameWidth, int gameHeight)
{
	this->startCol = startCol;
	this->startRow = startRow;
	this->gameWidth = gameWidth;
	this->gameHeight = gameHeight;

	// initial map and blockMap
	map = new bool*[gameHeight];
	blocksMap = new Blocks**[gameHeight / 2];
	for (int i = 0; i < gameHeight / 2; i++) {
		map[i] = new bool[gameWidth];
		for (int j = 0; j < gameWidth; j++) {
			map[i][j] = false;
		}
		blocksMap[i] = new Blocks*[gameWidth];
	}

	for (int i = gameHeight / 2; i < gameHeight; i++) {
		map[i] = new bool[gameWidth];
		for (int j = 0; j < gameWidth; j++) {
			map[i][j] = false;
		}
	}
}


GameController::~GameController()
{
	for (int i = 0; i < gameHeight / 2; i++) {
		delete map[i];
		delete blocksMap[i];
	}
	delete map;
	delete blocksMap;
}

void GameController::BoardGoRight()
{
	if (board->x + board->width + boardSpeed < gameWidth)
		board->x = board->x + boardSpeed;
	else
		board->x = gameWidth - board->width;
}

void GameController::BoardGoLeft()
{
	if (board->x - boardSpeed > 0)
		board->x = board->x - boardSpeed;
	else
		board->x = 0;
}


//set stage
void GameController::Initialize()
{
	int colors[] = {
		BACKGROUND_BLUE | BACKGROUND_INTENSITY,
		BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
		BACKGROUND_RED | BACKGROUND_INTENSITY,
		BACKGROUND_GREEN | BACKGROUND_INTENSITY,
		BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY
	};

	int colorCount = 0;
	int initPos = 0;
	int rowNum = gameHeight / 2;
	int blockWidth = 3;
	int space = 3;
	int cursor = 0;

	// create board & square, init blocksNum
	board = new Blocks(gameWidth / 2 - boardWidth / 2, gameHeight*0.9, boardWidth, BACKGROUND_RED | BACKGROUND_INTENSITY);
	square = new Square(gameWidth / 2, gameHeight*0.7, BACKGROUND_RED | BACKGROUND_INTENSITY, 1);
	blocksNum = 0;

	// create blocks with different colors and pos and record in map
	for (int i = 1; i < rowNum; i++) {

		if (initPos == 0)
			initPos = space;
		else
			initPos = 0;

		while ((initPos + cursor + blockWidth) < gameWidth) {
			blocksMap[i][initPos + cursor] = new Blocks(initPos + cursor, i, blockWidth, colors[colorCount]);
			map[i][initPos + cursor] = true;
			blocksNum += 1;
			// fill blocks map for blockWidth
			for (int k = 1; k < blockWidth; k++) {
				blocksMap[i][initPos + cursor + k] = blocksMap[i][initPos + cursor];
				map[i][initPos + cursor + k] = true;
			}
			cursor += space + blocksMap[i][initPos + cursor]->width;
		}

		colorCount++;
		if (colorCount == (sizeof(colors) / sizeof(int))) {
			colorCount = 0;
		}
		cursor = 0;
	}
}

void GameController::Render()
{
	if (end == true) {

		Initialize();
		end = false;

		// clean screen and move to init pos
		system("cls");
		gotoxy(startCol, startRow);

		//draw blocks
		int y = 0;
		int cursor = 0;
		while (y < gameHeight) {
			if (map[y][cursor] == true && blocksMap[y][cursor]->enabled == true) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), blocksMap[y][cursor]->color);
				for (int i = 0; i < blocksMap[y][cursor]->width; i++) {
					printf(" ");
				}
				cursor += blocksMap[y][cursor]->width;
			}
			else {
				cursor += 1;
			}

			if (cursor >= gameWidth) {
				y += 1;
				cursor = 0;
			}
			gotoxy(cursor + startCol, y + startRow);
		}

		//draw border
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		gotoxy(startCol - 1, startRow - 1);
		for (int i = 0; i < gameWidth + 3; i++) {
			printf("o");
		}
		for (int i = 0; i < gameHeight; i++) {
			gotoxy(startCol - 1, startRow + i);
			printf("o");
			gotoxy(startCol + gameWidth + 1, startRow + i);
			printf("o");
		}

		//write word
		char c[] = "Press Enter To Begin";
		ShowWord(c);
		char t = getchar();
		char d[] = "Fight !               ";
		ShowWord(d);
	}

	//draw square	
	gotoxy(square->x + startCol, square->y + startRow);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), square->color);
	printf(" ");

	//draw board
	gotoxy(board->x + startCol, board->y + startRow);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), board->color);
	for (int i = 0; i < board->width; i++) {
		printf(" ");
	}

	// reset bgColor and set final cursor pos
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NULL);
	gotoxy(gameWidth + startCol, gameHeight + startRow);
}
void GameController::Update()
{
	//update square pos
	Erase(*square);
	if (square->dir == 0) {
		square->x -= 1;
		square->y -= 1;
	}
	else if (square->dir == 1) {
		square->x += 1;
		square->y -= 1;
	}
	else if (square->dir == 2) {
		square->x += 1;
		square->y += 1;
	}
	else if (square->dir == 3) {
		square->x -= 1;
		square->y += 1;
	}

	// check win and lose
	if (square->y >= gameHeight + 5)
		Lose();
	else if (blocksNum == 0)
		Victory();

	//update board pos
	Erase(*board);
	if (GetKeyState(VK_LEFT) < 0) {
		BoardGoLeft();
	}
	else if (GetKeyState(VK_RIGHT) < 0) {
		BoardGoRight();
	}
}

void GameController::CheckCollide()
{
	int x = square->x;
	int y = square->y;
	Graphic *collider = NULL;

	// if touch border
	if (x <= 0) {		// if touch left border
		collider = new Graphic(startCol - 1, y);
	}
	else if (x >= gameWidth) {		// if touch right border
		collider = new Graphic(startCol + gameWidth + 1, y);
	}
	else if (y <= 0) {		// if touch upper border
		collider = new Graphic(startCol + 3, y - 1);
	}

	if (collider != NULL) {
		square->Collide(*collider);
	}

	// if touch blocks
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (y + i > 0 && x + j > 0 && y + i < gameHeight && x + j < gameWidth) {
				if (map[y + i][x + j] == true && blocksMap[y + i][x + j]->enabled == true) {
					square->Collide(*blocksMap[y + i][x + j]);
					blocksMap[y + i][x + j]->Collide(*square);
					map[y + i][x + j] = false;
					blocksNum -= 1;
					Erase(*blocksMap[y + i][x + j]);
				}
			}
		}
	}

	//if touch board
	if (board->y == square->y + 1) {
		if (square->x >= board->x - 1 && square->x <= (board->x + board->width))
			square->Collide(*board);
	}
}

// move cursor to (x,y)
void GameController::gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void GameController::Lose()
{
	char c[] = "You Lose , press enter to retry";
	ShowWord(c);
	this->end = true;
	int t = getchar();
}

void GameController::Victory()
{
	char c[] = "Victory ! press enter to retry";
	ShowWord(c);
	this->end = true;
	int t = getchar();
}

void GameController::ShowWord(char* set)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	gotoxy(startCol + startTxtCol, startRow + startTxtRow);
	printf(set);
}

// erase blocks on cmd screen
void GameController::Erase(Blocks blocks)
{
	gotoxy(blocks.x + blocks.width + startCol, blocks.y + startRow);
	for (int i = 0; i < blocks.width; i++)
	{
		printf("\b");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NULL);
	for (int i = 0; i < blocks.width; i++)
	{
		printf(" ");
	}
	gotoxy(gameWidth, gameHeight);
}

// erase square on cmd screen
void GameController::Erase(Square square)
{
	gotoxy(square.x + startCol + 1, square.y + startRow);
	printf("\b");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NULL);
	printf(" ");
	gotoxy(gameWidth, gameHeight);

}
