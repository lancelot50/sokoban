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
#include <assert.h>

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

	class Storage
	{
		enum BlockType
		{
			WALL,
			EMPTY_SLOT,
			GOAL,
			BOX,
			BOX_ON_THE_GOAL,
			PLAYER,
			PLAYER_ON_THE_GOAL
		};

		class Player
		{
			int m_PosIdx;
		public:
			Player() :m_PosIdx(0) {}
			void SetPos(int PosIdx) { m_PosIdx = PosIdx; }
			int GetPos() const { return m_PosIdx; }
		};

		BlockType* m_StorageArray;

		int m_Width;
		int m_Height;
		int m_Size;

		Player m_Player;

		void createWall()
		{
			for (int i = 0; i < m_Width; ++i)
			{
				for (int j = 0; j < m_Height; ++j)
				{
					assert(i*m_Height + j < m_Size);		// code analysis 오류나서

					if (i == 0 || j == 0 || j == m_Height - 1 || i == m_Width - 1)
						m_StorageArray[i*m_Height + j] = WALL;
					else
						m_StorageArray[i*m_Height + j] = EMPTY_SLOT;
				}
			}
		}

		void loadBox()
		{
			for (int i = 0; i < m_Width; ++i)
			{
				for (int j = 0; j < m_Height; ++j)
				{
					assert(i*m_Height + j < m_Size);		// code analysis 오류나서

					if (m_StorageArray[i*m_Height + j] != WALL && rand() % 10 == 0)
					{
						m_StorageArray[i*m_Height + j] = BOX;
					}
				}
			}
		}

		void loadGoal()
		{

		}

	public :
		Storage() : m_Width(0), m_Height(0), m_Size(0), m_StorageArray(0) {}
		virtual ~Storage()
		{
			delete[] m_StorageArray;
		}

		int GetSize() const { return m_Size; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetPlayerIndex() const { return m_Player.GetPos(); }

		void Init(int Width, int Height)
		{
			m_Width = Width;
			m_Height = Height;
			m_Size = Width*Height;
			m_StorageArray = new BlockType[m_Size];

			createWall();
			loadBox();
			loadGoal();
		}

		void InitPlayerIndex()
		{
			srand(static_cast<unsigned int>(time(NULL)));

			int pos = rand() % m_Size;
			if (m_StorageArray[pos] == EMPTY_SLOT)
			{
				m_StorageArray[pos] = PLAYER;
			}
			else if (pos < m_Width)
				m_StorageArray[pos + m_Width] = PLAYER;
			else if (pos > m_Size - m_Width)
				m_StorageArray[pos - m_Width] = PLAYER;

			m_Player.SetPos(pos);
		}

		void PlayerMoveLeft()
		{
			int playerLeftIndex = m_Player.GetPos() - 1;
			if (m_StorageArray[playerLeftIndex] == EMPTY_SLOT)
			{
				swapIndex(m_Player.GetPos(), playerLeftIndex);
				wcout << L"왼쪽이동" << endl;
				m_Player.SetPos(playerLeftIndex);
			}
			else
				wcout << L"왼쪽이동불가" << endl;
		}

		void PlayerMoveRight()
		{
			int playerRightIndex = m_Player.GetPos() + 1;
			if (m_StorageArray[playerRightIndex] == EMPTY_SLOT)
			{
				swapIndex(m_Player.GetPos(), playerRightIndex);
				wcout << L"오른쪽이동" << endl;
				m_Player.SetPos(playerRightIndex);
			}
			else
				wcout << L"오른쪽이동불가" << endl;

		}
		void PlayerMoveUp()
		{
			int playerUpIndex = m_Player.GetPos() - m_Height;
			if (playerUpIndex > 0 && m_StorageArray[playerUpIndex] == EMPTY_SLOT)
			{
				swapIndex(m_Player.GetPos(), playerUpIndex);
				wcout << L"위쪽이동" << endl;
				m_Player.SetPos(playerUpIndex);
			}
			else
				wcout << L"위쪽이동불가" << endl;

		}
		void PlayerMoveDown()
		{
			int playerDownIndex = m_Player.GetPos() + m_Height;
			if (playerDownIndex < m_Size && m_StorageArray[playerDownIndex] == EMPTY_SLOT)
			{
				swapIndex(m_Player.GetPos(), playerDownIndex);
				wcout << L"아래쪽이동" << endl;
				m_Player.SetPos(playerDownIndex);
			}
			else
				wcout << L"아래쪽이동불가" << endl;
		}

		void swapIndex(int PlayerIndex, int MovetoIndex) const
		{
			BlockType temp = m_StorageArray[MovetoIndex];
			m_StorageArray[MovetoIndex] = PLAYER;
			m_StorageArray[PlayerIndex] = temp;
		}

		void Draw() const
		{
			for (int i = 0; i<m_Width; ++i)
			{
				for (int j = 0; j<m_Height; ++j)
				{
					switch (m_StorageArray[i*m_Height + j])
					{
					case WALL:
						wcout << L'|';
						break;
					case EMPTY_SLOT:
						wcout << L' ';
						break;
					case GOAL:
						wcout << L'.';
						break;
					case BOX:
						wcout << L'o';
						break;
					case BOX_ON_THE_GOAL:
						wcout << L'O';
						break;
					case PLAYER:
						wcout << L'p';
						break;
					case PLAYER_ON_THE_GOAL:
						wcout << L'P';
						break;
					default:
						break;
					}
				}
				wcout << endl;
			}
		}
	};

	Storage m_Storage;

public:
	Game() { }
	void Start(int width, int height)
	{
		initialize(width, height);
		gameLoop();
		terminate();
	}

private:

	void terminate() {	}

	void initialize(int Width, int Height)
	{
		m_Storage.Init(Width, Height);
		m_Storage.InitPlayerIndex();
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

	void movePlayerPosition(MoveAction move)
	{
		switch (move)
		{
		case LEFT:
			m_Storage.PlayerMoveLeft();
			break;
		case RIGHT:
			m_Storage.PlayerMoveRight();
			break;
		case UP:
			m_Storage.PlayerMoveUp();
			break;
		case DOWN:
			m_Storage.PlayerMoveDown();
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
		m_Storage.Draw();
	}

	void drawDebugInfo() const
	{
		drawPlayerIndex();
		drawStorageInfo();
		wcout << endl;
	}

	void drawStorageInfo() const
	{
		wcout << L"StorageWidth : " << m_Storage.GetWidth() << L", StorageHeight:" << m_Storage.GetHeight() << endl;
	}

	void drawPlayerIndex() const
	{
		wcout << L"플레이어Index:" << m_Storage.GetPlayerIndex() << endl;
	}

	void clear()
	{
		system("cls");
	}
};

int main()
{
	wcout.imbue(locale("kor"));

	Game game;
	game.Start(16,8);
	return 0;
}