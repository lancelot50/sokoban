//#include<iostream>
//#include<cstdlib>
//
//using namespace std;
//
//wchar_t* StorageArray[]=
//{
//	L"########",
//	L"#......#",
//	L"#......#",
//	L"#......#",
//	L"#......#",
//	L"#......#",
//	L"#......#",
//	L"########"
//};
//
//class Sokoban
//{
//	static const int STORAGE_WIDTH=8;
//	static const int STORAGE_HEIGHT=8;
//	int m_Storage[STORAGE_WIDTH][STORAGE_HEIGHT];
//
//	bool IsExitCondition(wchar_t input)
//	{
//		if(input==L'q')
//			return true;
//		else
//			return false;
//	}
//	
//	wchar_t GetInput()
//	{
//		wchar_t input;
//		wcin>>input;
//		wcout<<L"input num : "<<input<<endl;
//		return input;
//	}
//
//	void Update()
//	{
//	}
//	void ClearScreen()
//	{
//		system("cls");
//	}
//	void DrawStorage()
//	{
//		//int arrLen=sizeof(StorageArray)/sizeof(StorageArray[0]);
//		//for(int i=0; i<arrLen; ++i)
//		//{
//		//	wcout<< StorageArray[i]<<endl;
//		//}
//		for(int i=0; i<STORAGE_WIDTH; ++i)
//		{
//			for(int j=0; j<STORAGE_HEIGHT; ++j)
//			{
//				wcout<<m_Storage[i][j];
//			}
//			cout<<endl;
//		}
//	}
//	void DrawMan()
//	{
//
//	}
//	void Draw()
//	{
//		ClearScreen();
//		DrawStorage();
////		DrawMan();
//	}
//
//	void LoadStorage()
//	{
//	}
//
//public :
//	Sokoban()
//	{
//		for(int i=0; i<STORAGE_WIDTH; ++i)
//		{
//			for(int j=0; j<STORAGE_HEIGHT; ++j)
//			{
//				m_Storage[i][j]=0;
//			}
//		}
//
//	}
//	void Start()
//	{
//		wchar_t input=0;
////		wcout<<L"size:"<<sizeof(StorageArray);
////		wcout<<L"size:"<<sizeof(StorageArray[0]);
//		LoadStorage();
//		while(true)
//		{
//			input=GetInput();
//
//			if( IsExitCondition(input) )
//				break;
//			Update();
//			ClearScreen();
//			Draw();
//		}
//
//	}
//};
//
//int main()
//{
//	Sokoban game;
//	game.Start();
//
//	return 0;
//}
#include<iostream>
#include<string>
using namespace std;
#include<stdlib.h>


class Game
{
	enum MoveAction
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	enum BlockType
	{
		WALL,
		FREE_SLOT,
		EMPTY_SLOT,
		FILLED_SLOT,
		PLAYER
	};

	BlockType* m_Storage;

	int m_StorageWidth;
	int m_StorageHeight;


public :
	void Start(int width, int height)
	{
		initialize(width, height);
		gameLoop();
		terminate();
	}

private :

	void terminate()
	{
		delete[] m_Storage;
	}

	void initialize(int Width, int Height)
	{
		m_Storage=new BlockType[Width*Height];

		m_StorageWidth=Width;
		m_StorageHeight=Height;

		for(int i=0; i<Width; ++i)
		{
			for(int j=0; j<Height; ++j)
			{
				if(i==0 || j==0 || j==Height-1 || i==Width-1 )
					m_Storage[i*Height+j]=WALL;
				else
					m_Storage[i*Height+j]=EMPTY_SLOT;
			}
		}
	}

	void gameLoop()
	{
		while(true)
		{
			draw();
			wchar_t input=getInput();
			if( input==L'q')
				break;
			update(input);			
		}
	}
	wchar_t getInput()
	{
		wchar_t inputChar=0;
		wcin>>inputChar;
		return inputChar;
	}

	void update(wchar_t input)
	{
		movePlayerPosition(input);
	}

	void movePlayerPosition(wchar_t input)
	{
		MoveAction move=LEFT;

		switch(move)
		{
		case LEFT :
//			moveLeft();
			wcout<<L"왼쪽이동"<<endl;
			break;
		case RIGHT :
			break;
		case UP :
			break;
		case DOWN :
			break;
		default :
			wcout<<L"정해지지않은입력:"<<move<<endl;
			wcin.get();
		}
	}
	
	void drawPlayer()
	{
	}

	void draw()
	{
		clear();
		drawStorage();
		drawPlayer();
	}

	void clear()
	{
		system("cls");
	}
	void drawStorage()
	{
		for(int i=0; i<m_StorageWidth; ++i)
		{
			for(int j=0; j<m_StorageHeight; ++j)
			{
				switch(m_Storage[i*m_StorageHeight+j])
				{
				case WALL :
					wcout<<L'|';
					break;
				case EMPTY_SLOT :
					wcout<<L'.';
					break;
				}
			}
			wcout<<endl;
		}
	}
};

int main()
{
	Game game;
	game.Start(8,10);
	return 0;
}