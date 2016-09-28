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

#include<locale>

class Game
{
	enum MoveAction
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		HOLD
	};

	enum BlockType
	{
		WALL,
		FREE_SLOT,
		EMPTY_SLOT,
		FILLED_SLOT,
		PLAYER
	};

	class Player
	{
		int m_PosIdx;
	public:
		Player() :m_PosIdx(0) {}
		void SetPos(int PosIdx) { m_PosIdx = PosIdx; }
		int GetPos() const { return m_PosIdx; }
	};

	class Storage
	{

	};

	BlockType* m_Storage;

	int m_StorageWidth;
	int m_StorageHeight;
	int m_StorageSize;

	Player m_Player;

public:
	Game() : m_Storage(NULL), m_StorageWidth(0), m_StorageHeight(0), m_StorageSize(0) { }
	void Start(int width, int height)
	{
		initialize(width, height);
		gameLoop();
		terminate();
	}

private:

	void terminate()
	{
		delete[] m_Storage;
	}

	void initialize(int Width, int Height)
	{
		m_Storage = new BlockType[Width*Height];

		m_StorageWidth = Width;
		m_StorageHeight = Height;
		m_StorageSize = Width*Height;

		for (int i = 0; i < Width; ++i)
		{
			for (int j = 0; j < Height; ++j)
			{
				if (i == 0 || j == 0 || j == Height - 1 || i == Width - 1)
					m_Storage[i*Height + j] = WALL;
				else
					m_Storage[i*Height + j] = EMPTY_SLOT;
			}
		}

		initPlayerIndex();
	}

	void initPlayerIndex()
	{
		int size = m_StorageWidth*m_StorageHeight;

		srand(static_cast<unsigned int>(time(NULL)));
		int pos = rand() % size;
		if (m_Storage[pos] == EMPTY_SLOT)
		{
			m_Storage[pos] = PLAYER;
		}
		else if (pos < m_StorageWidth)
			m_Storage[pos + m_StorageWidth] = PLAYER;
		else if (pos > size - m_StorageWidth)
			m_Storage[pos - m_StorageWidth] = PLAYER;

		m_Player.SetPos(pos);
	}

	void gameLoop()
	{
		while (true)
		{
			draw();
			wchar_t input = getInput();
			if (input == L'q')
				break;
			update(input);
		}
	}
	wchar_t getInput()
	{
		wchar_t inputChar = 0;
		wcin >> inputChar;
		return inputChar;
	}

	MoveAction GetMoveAction(wchar_t input)
	{
		MoveAction result = HOLD;
		switch (input)
		{
		case 'a':
			result = LEFT;
			break;
		case 'd':
			result = RIGHT;
			break;
		case 'w':
			result = UP;
			break;
		case 'x':
			result = DOWN;
			break;
		default:
			wcout << L"정해지지않은입력:" << input << endl;
			wcin.get();
		}

		return result;
	}

	void update(wchar_t input)
	{
		MoveAction move = GetMoveAction(input);
		movePlayerPosition(move);
	}

	int findPlayerIndex() const
	{
		int playerIndex = 0;
		for (int i = 0; i < m_StorageSize; ++i)
		{
			if (m_Storage[i] == PLAYER)
			{
				playerIndex = i;
				break;
			}
		}

		return playerIndex;
	}

	void swapIndex(int PlayerIndex, int MovetoIndex) const
	{
		BlockType temp = m_Storage[MovetoIndex];
		m_Storage[MovetoIndex] = PLAYER;
		m_Storage[PlayerIndex] = temp;
	}
	void moveLeft(int PlayaerIndex)
	{
		int playerLeftIndex = PlayaerIndex - 1;
		if (m_Storage[playerLeftIndex] == EMPTY_SLOT)
		{
			swapIndex(PlayaerIndex, playerLeftIndex);
			wcout << L"왼쪽이동" << endl;
		}
		else
			wcout << L"왼쪽이동불가" << endl;
	}

	void moveRight(int PlayerIndex)
	{
		int playerRightIndex = PlayerIndex + 1;
		if (m_Storage[playerRightIndex] == EMPTY_SLOT)
		{
			swapIndex(PlayerIndex, playerRightIndex);
			wcout << L"오른쪽이동" << endl;
		}
		else
			wcout << L"오른쪽이동불가" << endl;

	}
	void moveUp(int PlayerIndex)
	{
		int playerUpIndex = PlayerIndex - m_StorageHeight;
		if (playerUpIndex > 0 && m_Storage[playerUpIndex] == EMPTY_SLOT)
		{
			swapIndex(PlayerIndex, playerUpIndex);
			wcout << L"위쪽이동" << endl;
		}
		else
			wcout << L"위쪽이동불가" << endl;

	}
	void moveDown(int PlayerIndex)
	{
		int playerDownIndex = PlayerIndex + m_StorageHeight;
		if (playerDownIndex < m_StorageSize && m_Storage[playerDownIndex] == EMPTY_SLOT)
		{
			swapIndex(PlayerIndex, playerDownIndex);
			wcout << L"아래쪽이동" << endl;
		}
		else
			wcout << L"아래쪽이동불가" << endl;
	}



	void movePlayerPosition(MoveAction move)
	{
		int playerIndex = findPlayerIndex();
		switch (move)
		{
		case LEFT:
			moveLeft(playerIndex);
			break;
		case RIGHT:
			moveRight(playerIndex);
			break;
		case UP:
			moveUp(playerIndex);
			break;
		case DOWN:
			moveDown(playerIndex);
			break;
		default:
			wcout << L"정해지지않은움직임:" << move << endl;
			wcin.get();
		}
	}

	void draw()
	{
		clear();
		drawDebugInfo();
		drawStorage();
	}

	void drawDebugInfo() const
	{
		drawPlayerIndex();
		drawStorageInfo();
		wcout << endl;
	}

	void drawStorageInfo() const
	{
		wcout << L"StorageWidth : " << m_StorageWidth << L", StorageHeight:" << m_StorageHeight << endl;
	}

	void drawPlayerIndex() const
	{
		wcout << L"플레이어Index:" << findPlayerIndex() << endl;
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
				case PLAYER :
					wcout << L'P';
					break;
				default:
					break;
				}
			}
			wcout<<endl;
		}
	}
};

int main()
{
	wcout.imbue(locale("kor"));

	Game game;
	game.Start(16,8);
	return 0;
}