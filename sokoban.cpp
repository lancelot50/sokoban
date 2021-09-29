#include"GameLib\Framework.h"

using namespace GameLib;

#include<iostream>
#include<string>
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
		std::cout << "******************" << endl;
		std::cout << "     You Win!" << endl;
		std::cout << "******************" << endl;
		std::cout << "**** continue(y/n)? ***" << endl;
	}
	void drawInputMessage()
	{
		std::cout << "*************************************" << endl;
		std::cout << "LEFT:a, RIGHT:d, UP:w, DOWN:x, QUIT:q" << endl;
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
		std::cin >> inputChar;
		return inputChar;
	}

	char getRealTimeInput()
	{
		enum Char
		{
			A,D,W,X,Max_Char
		};
		static bool prevInput[Max_Char] = { false, false, false, false };
		bool inputChar[Max_Char];
		inputChar[A] = Framework::instance().isKeyOn('a');
		inputChar[D] = Framework::instance().isKeyOn('d');
		inputChar[W] = Framework::instance().isKeyOn('w');
		inputChar[X] = Framework::instance().isKeyOn('x');
		char inChar = 0;
		if (inputChar[A] && !prevInput[A])
			inChar = 'a';
		else if (inputChar[D] && !prevInput[D])
			inChar = 'd';
		if (inputChar[W] && !prevInput[W])
			inChar = 'w';
		else if (inputChar[X] && !prevInput[X])
			inChar = 'x';
		for( int i=0; i<Max_Char; ++i)
			prevInput[i] = inputChar[i];

		return inChar;
	}

	void drawGameInfo()
	{
		std::cout << "Player Move Count : "<<m_Level.PlayerMoveCnt()<<endl;
	}

	void drawDebugInfo()
	{
		std::cout << endl;
		std::cout << "플레이어 Pos(" << m_Level.PlayerPosX() << ", " << m_Level.PlayerPosY() << ")" << endl;
		std::cout << "StorageWidth : " << m_Level.Width() << ", StorageHeight:" << m_Level.Height() << ",  BoxCnt:" << m_Level.BoxCnt() << endl;
		std::cout << endl << "PrevFrameLog : " << m_Level.PrevFrameLog() << endl;
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
//		clearScreen();
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
		if (m_Level.IsComplete())
		{
			reset();
			return false;
		}
		char input = getRealTimeInput();
		m_Level.Update(input);
		return false;
	}
};

//#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

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