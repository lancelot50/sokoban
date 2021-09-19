#include"GameLib\Framework.h"

using namespace GameLib;
//using namespace std;

//#include<iostream>
#include<string>
using namespace std;
#include<sstream>


#include "array2dTemplate.h"
#include "player.h"

#include "GameConstant.h"

#include "RenderInterface.h"
#include "ConsoleRenderInterface.h"
#include "D3DRenderInterface.h"

#include "Level.h"

class Game
{
	Storage m_Level;

	RenderInterface* RI=nullptr;
	RenderInterface* RI2=nullptr;

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
		cout << "Player Move Count : "<<m_Level.PlayerMoveCnt()<<endl;
	}

	void drawDebugInfo()
	{
		cout << endl;
		cout << "플레이어 Pos(" << m_Level.PlayerPosX() << ", " << m_Level.PlayerPosY() << ")" << endl;
		cout << "StorageWidth : " << m_Level.Width() << ", StorageHeight:" << m_Level.Height() << ",  BoxCnt:" << m_Level.BoxCnt() << endl;
		cout << endl << "PrevFrameLog : " << m_Level.PrevFrameLog() << endl;
		m_Level.ClearLog();
	}

	void clearScreen() const {	system("cls");	}

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
		RI = new D3DRenderInterface();
		RI2 = new ConsoleRenderInterface();

		srand(static_cast<unsigned int>(time(NULL)));

		m_Level.Create(Width, Height);
		m_Level.Init();
	}
	void Terminate()
	{
		delete RI;
		delete RI2;
	}

	void Draw()
	{
		clearScreen();
		drawGameInfo();
		m_Level.Render(RI);
		m_Level.Render(RI2);
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

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
namespace GameLib
{
	void Framework::update()
	{
		static Game game;
		static bool bInit = false;
		if (!bInit)
		{
			game.Initialize(8, 7);
			game.Draw();
			bInit = true;
		}
		else
		{
			bool bQuit=game.Update();
			game.Draw();
			if (bQuit)
				Framework::instance().requestEnd();
			if (Framework::instance().isEndRequested())
			{
				game.Terminate();
			}
		}
	}
}