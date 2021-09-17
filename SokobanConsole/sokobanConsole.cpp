#include<iostream>
#include<string>
using namespace std;

#include<sstream>

#include "../array2dTemplate.h"
#include "../player.h"

#include "../GameConstant.h"

#include "../RenderInterface.h"
#include "../ConsoleRenderInterface.h"

#include "../Level.h"


class Game
{
	Storage m_Level;

	RenderInterface* RI = nullptr;

	void reset()
	{
		m_Level.Init();
	}

	void drawWinMessage()
	{
		cout << "******************" << endl;
		cout << "     You Win!" << endl;
		cout << "******************" << endl;
		cout << "**** continue(y/n)? ***" << endl;
	}
	void drawInputMessage()
	{
		cout << "*************************************" << endl;
		cout << "LEFT:a, RIGHT:d, UP:w, DOWN:x, QUIT:q" << endl;
	}

	void gameLoop()
	{
		while (true)
		{
			Draw();

			bool bQuit = Update();
			if (bQuit)
				break;
		}
	}

	char getInput()
	{
		char inputChar = 0;
		cin >> inputChar;
		return inputChar;
	}

	void drawGameInfo()
	{
		cout << "Player Move Count : " << m_Level.GetPlayerMoveCnt() << endl;
	}

	void drawDebugInfo()
	{
		cout << endl;
		cout << "플레이어 Pos(" << m_Level.GetPlayerPosX() << ", " << m_Level.GetPlayerPosY() << ")" << endl;
		cout << "StorageWidth : " << m_Level.GetWidth() << ", StorageHeight:" << m_Level.GetHeight() << ",  BoxCnt:" << m_Level.GetBoxCnt() << endl;
		cout << endl << "PrevFrameLog : " << m_Level.GetPrevFrameLog() << endl;
		m_Level.ClearLog();
	}

	void clearScreen() const {	system("cls"); }

public:
	Game() { }
	void Start(int width, int height)
	{
		Initialize(width, height);
		gameLoop();
		Terminate();
	}

	void Initialize(int Width, int Height)
	{
		RI = new ConsoleRenderInterface();
		srand(static_cast<unsigned int>(time(NULL)));

		m_Level.Create(Width, Height);
		m_Level.Init();
	}
	void Terminate()
	{
		delete RI;
	}

	void Draw()
	{
		clearScreen();
		drawGameInfo();
		m_Level.Render(RI);
		cout << endl;
		drawDebugInfo();

		if (m_Level.IsComplete())
			drawWinMessage();
		else
			drawInputMessage();
	}

	bool Update()
	{
		char input = getInput();
		if (input == 'q')
			return true;
		else if (m_Level.IsComplete())
		{
			if (input == 'y')
			{
				reset();
				return false;
			}
			else
				return true;
		}

		m_Level.Update(input);
		return false;
	}

};

#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
int main()
{
	Game game;
	game.Start(8,8);
	return 0;
}