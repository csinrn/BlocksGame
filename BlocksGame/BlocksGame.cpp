// BlocksGame.cpp: 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "Windows.h"
#include "GameController.h"

int main()
{
	GameController gameController(3, 3, 25, 20);
	gameController.Render();

	while (true)
	{
		gameController.CheckCollide();
		gameController.Update();
		gameController.Render();
		Sleep(100);
	}

	return 0;
}

