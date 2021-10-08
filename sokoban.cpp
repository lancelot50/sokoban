#include"GameLib\Framework.h"

using namespace GameLib;

//#include<iostream>
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

//	Sequence* m_Seq=nullptr;

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
//		cin >> inputChar;
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
	Game()
	{
//		m_Seq = new SequenceTitle;
	}
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

class Screen
{
public :
	virtual void Update() = 0;
};

class TitleScreen : public Screen
{
	Image* m_pTitle = nullptr;
public:
	TitleScreen()
	{
		m_pTitle = new Image("title.dds");
	}
	virtual ~TitleScreen()
	{
		delete m_pTitle;
	}
	void Update()
	{
		if (m_pTitle)
			m_pTitle->Draw();
		bool bSpaceKeyPressed = Framework::instance().isKeyTriggered(' ');
		if (bSpaceKeyPressed)
		{
		}
	}
};

class StageSelectScreen : public Screen
{
	Image* m_pStageSelect = nullptr;
public:
	StageSelectScreen()
	{
		m_pStageSelect = new Image("title.dds");
	}
	virtual ~StageSelectScreen()
	{
		delete m_pStageSelect;
	}
	void Update()
	{
		if (m_pStageSelect)
			m_pStageSelect->Draw();
		bool bSpaceKeyPressed = Framework::instance().isKeyTriggered(' ');
		if (bSpaceKeyPressed)
		{

		}
	}
};

class GameScreen : public Screen
{
	void Update()
	{
		bool bQKeyPressed = Framework::instance().isKeyTriggered('q');
		if (bQKeyPressed)
		{
			cout << "GameScreen" << endl;
		}
	}
};



namespace GameLib
{
	void Framework::update()
	{
		//static Game game;
		//static bool bInit = false;
		//if (!bInit)
		//{
		//	game.Initialize(8, 7);
		//	game.Draw();
		//	bInit = true;
		//}
		//else
		//{
		//	bool bQuit=game.Update();
		//	game.Draw();
		//	if (bQuit)
		//		Framework::instance().requestEnd();
		//	if (Framework::instance().isEndRequested())
		//	{
		//		game.Terminate();
		//	}
		//}

		static Screen* GameSeq;
		if (GameSeq == nullptr)
		{
			GameSeq = new TitleScreen;
		}
		GameSeq->Update();
//		switch (GameSeq->NextScreen())
		{

		}

	}
}