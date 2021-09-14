#include"GameLib\Framework.h"

using namespace GameLib;
//using namespace std;


//#include<iostream>
#include<string>
using namespace std;
#include<stdlib.h>

#include<locale>
#include <assert.h>

#include<sstream>

template<class T>
class Array2D
{
	T* m_Array=nullptr;
	int m_Width = 0;
	int m_Height = 0;

	void destroy()
	{
		if (m_Array)
		{
			delete[] m_Array;
			m_Array = nullptr;
		}
	}

public :
	Array2D(int Width, int Height)
	{
		SetSize(Width, Height);
	}
	void SetSize(int Width, int Height)
	{
		destroy();
		m_Width = Width;
		m_Height = Height;
		m_Array = new T[m_Width * m_Height];
	}
	void Fill(T element)
	{
		for (int i = 0; i < m_Width * m_Height; ++i)
			m_Array[i] = element;
	}
	virtual ~Array2D()
	{
		destroy();
	}

	T& operator()(int X, int Y)
	{
		return m_Array[Y * m_Width + X];
	}
	const T& operator()(int X, int Y) const
	{
		return m_Array[Y * m_Width + X];
	}
};




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
			EMPTY_SLOT = 0x00000000,
			WALL = 0x00000001,
			GOAL = 0x00000002,
			BOX = 0x00000004,
			PLAYER = 0x00000008,
			BOX_ON_THE_GOAL = BOX | GOAL,
			PLAYER_ON_THE_GOAL = PLAYER | GOAL,
			MAX_BLOCK_TYPE = PLAYER_ON_THE_GOAL + 1,
		};

		char m_StorageTextRenderer[MAX_BLOCK_TYPE] = { EMPTY_SLOT, };
		unsigned int m_StorageColorFiller[MAX_BLOCK_TYPE] = { EMPTY_SLOT, };

		class Player
		{
			int m_PosIdx;
			int m_MoveCnt;
			int m_PosX=0;
			int m_PosY=0;
		public:
			Player() :m_PosIdx(0), m_MoveCnt(0) {}
			void SetPos(int PosIdx)
			{
				m_PosIdx = PosIdx;
				++m_MoveCnt;
			}
			void SetPos(int PosX, int PosY)
			{
				m_PosX = PosX;
				m_PosY = PosY;
				++m_MoveCnt;
			}
			void SetPosX(int PosX)
			{
				m_PosX = PosX;
			}
			void SetPosY(int PosY)
			{
				m_PosY = PosY;
			}
			int GetPosX() const { return m_PosX; }
			int GetPosY() const { return m_PosY; }
			int GetPos() const { return m_PosIdx; }
			int GetMoveCnt() const { return m_MoveCnt; }
			void ResetMoveCnt() { m_MoveCnt = 0; }
		};

		Array2D<BlockType> m_Storage2D{ 8,8 };

		int m_Width;
		int m_Height;
		int m_Size;
		int m_BoxCnt;

		Player m_Player;

		ostringstream	m_PrevFrameLog;

		void createWall()
		{
			for (int j = 0; j < m_Height; ++j)
			{
				for (int i = 0; i < m_Width; ++i)
				{
					assert(j * m_Width + i < m_Size);		// code analysis 오류나서

					if (i == 0 || j == 0 || j == m_Height - 1 || i == m_Width - 1)
					{
						m_Storage2D(i, j) = WALL;
					}
					else
					{
						m_Storage2D(i, j) = EMPTY_SLOT;
					}
				}
			}
		}

		bool isEdge(int X, int Y) const
		{
			if (m_Storage2D(X-1, Y)!=WALL && m_Storage2D(X+1, Y)!=WALL && m_Storage2D(X, Y-1)!=WALL && m_Storage2D(X, Y+1)!=WALL)
				return false;
			else
				return true;
		}

		void loadBox()
		{
			while (m_BoxCnt <= 0)
			{
				for (int j = 0; j < m_Height; ++j)
				{
					for (int i = 0; i < m_Width; ++i)
					{
						int curIndex = j * m_Width + i;
						assert(curIndex < m_Size);		// code analysis 오류나서
						bool bCreateBox = (rand() % 10 == 0);

						if (m_Storage2D(i, j) != WALL && !isEdge(i, j) && bCreateBox)
						{
							m_Storage2D(i, j) = BOX;
							++m_BoxCnt;
						}
					}
				}
			}
		}

		void loadGoal()
		{
			int boxCnt = m_BoxCnt;
			while (boxCnt > 0)
			{
				int x = (rand() % (m_Width - 2)) + 1;
				int y = (rand() % (m_Height - 2)) + 1;

				if (m_Storage2D(x, y) == EMPTY_SLOT)
				{
					m_Storage2D(x, y) = GOAL;
					--boxCnt;
				}
			}
		}

	public:
		Storage() : m_Width(0), m_Height(0), m_Size(0), m_BoxCnt(0) {}
		virtual ~Storage()
		{
		}

		int GetSize() const { return m_Size; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetPlayerIndex() const { return m_Player.GetPos(); }
		int GetBoxCnt() const { return m_BoxCnt; }
		int GetPlayerMoveCnt() const { return m_Player.GetMoveCnt(); }
		string GetPrevFrameLog() const { return m_PrevFrameLog.str(); }
		void ClearLog() { m_PrevFrameLog.str(""); m_PrevFrameLog << endl;; }

		void createBlockRenderArray()
		{
			m_StorageTextRenderer[EMPTY_SLOT] = ' ';
			m_StorageTextRenderer[WALL] = '#';
			m_StorageTextRenderer[GOAL] = '.';
			m_StorageTextRenderer[BOX] = 'o';
			m_StorageTextRenderer[PLAYER] = 'p';
			m_StorageTextRenderer[BOX_ON_THE_GOAL] = 'O';
			m_StorageTextRenderer[PLAYER_ON_THE_GOAL] = 'P';
		}

		void createStorageColorFiller()
		{
			m_StorageColorFiller[EMPTY_SLOT] = 0x000000;
			m_StorageColorFiller[WALL] = 0xffffff;
			m_StorageColorFiller[GOAL] = 0x0000ff;
			m_StorageColorFiller[BOX] = 0xff0000;
			m_StorageColorFiller[PLAYER] = 0x00ff00;
			m_StorageColorFiller[BOX_ON_THE_GOAL] = m_StorageColorFiller[GOAL] | m_StorageColorFiller[BOX];
			m_StorageColorFiller[PLAYER_ON_THE_GOAL] = m_StorageColorFiller[GOAL] | m_StorageColorFiller[PLAYER];
		}

		void Create(int Width, int Height)
		{
			m_Width = Width;
			m_Height = Height;
			m_Size = Width * Height;
			m_Storage2D.SetSize(Width, Height);
			createBlockRenderArray();
			createStorageColorFiller();
		}

		void Init()
		{
			m_Storage2D.Fill(EMPTY_SLOT);
			m_BoxCnt = 0;
			createWall();
			loadBox();
			loadGoal();

			initPlayerIndex();
			m_Player.ResetMoveCnt();
		}

		void initPlayerIndex()
		{
			while (true)
			{
				int x = (rand() % (m_Width - 2)) + 1;
				int y = (rand() % (m_Height - 2)) + 1;
				if (m_Storage2D(x, y) == EMPTY_SLOT)
				{
					m_Storage2D(x,y) = PLAYER;
					m_Player.SetPos(x,y);
					break;
				}
			}
		}

		void printBlock(int DestX, int DestY)
		{
			m_PrevFrameLog.setf(ios_base::hex, ios_base::basefield);
			m_PrevFrameLog.setf(ios_base::showbase);
			m_PrevFrameLog << ", dest Block:" << m_Storage2D(DestX, DestY) << endl;;
			m_PrevFrameLog.unsetf(ios_base::hex);
		}

		//bool blockHasPlayer(int Index) const
		//{
		//	if (m_StorageArray[Index] & PLAYER)
		//		return true;
		//	else
		//		return false;
		//}

		//bool blockHasBox(int Index) const
		//{
		//	if (m_StorageArray[Index] & BOX)
		//		return true;
		//	else
		//		return false;
		//}


		void PlayerMove(MoveAction Move)
		{
			int dx = 0;
			int dy = 0;
			switch (Move)
			{
			case LEFT :
				dx = -1;
				break;
			case RIGHT :
				dx = 1;
				break;
			case UP :
				dy = -1;
				break;
			case DOWN :
				dy = 1;
				break;
			}

			int srcX = m_Player.GetPosX();
			int srcY = m_Player.GetPosY();
			int destX = srcX+dx;
			int destY = srcY+dy;
			
			m_PrevFrameLog << "PlayerMove (" << srcX << ", " << srcY << ") -> (" << destX << ", " << destY << ")";
			printBlock(destX, destY);
			// 1. 목적지가 wall 이면 return false
			// 2. 목적지에 box 가 있으면 box 진행 방향으로 1칸 더 앞을 확인해보고 가능하다면(wall 아님, box 아님) 박스를 옮기고 플레이어도 옮긴다.
			// 3. 아니면 목적지로 플레이어를 이동시킨다.
			if (m_Storage2D(destX, destY) & WALL)
			{
				m_PrevFrameLog << "WALL에 막힘" << endl;
				return;
			}
			else if (m_Storage2D(destX, destY) & BOX)
			{
				m_PrevFrameLog << "앞에 BOX" << endl;
				int destDestX = destX + dx;
				int destDestY = destY + dy;
				bool isWall = m_Storage2D(destDestX, destDestY) & WALL;
				bool isBox = m_Storage2D(destDestX, destDestY) & BOX;
				if (!isWall && !isBox)
				{
					m_Storage2D(destDestX, destDestY) = static_cast<BlockType>(m_Storage2D(destDestX, destDestY) ^ BOX);
					m_Storage2D(destX, destY) = static_cast<BlockType>(m_Storage2D(destX, destY)^BOX);
					m_Storage2D(destX, destY) = static_cast<BlockType>(m_Storage2D(destX, destY)^PLAYER);
					m_Storage2D(srcX, srcY) = static_cast<BlockType>(m_Storage2D(srcX, srcY)^PLAYER);
					m_Player.SetPos(destX, destY);
				}
			}
			else
			{
				m_Storage2D(destX, destY) = static_cast<BlockType>(m_Storage2D(destX, destY) ^ PLAYER);
				m_Storage2D(srcX, srcY) = static_cast<BlockType>(m_Storage2D(srcX, srcY) ^ PLAYER);
				m_Player.SetPos(destX, destY);
			}

		}

		bool IsComplete() const
		{
			int matchCnt = 0;
			for (int j = 0; j < m_Height; ++j)
			{
				for( int i = 0; i < m_Width; ++i)
				{
				if (m_Storage2D(i, j) == BOX_ON_THE_GOAL)
					++matchCnt;
				}
			}

			if (matchCnt == m_BoxCnt)
				return true;
			else
				return false;
		}

		void drawCell(int X, int Y, unsigned int Color) const
		{
			unsigned* vram = Framework::instance().videoMemory();
			unsigned windowWidth = Framework::instance().width();
			static const int cellSize = 16;

			for (int i = 0; i < cellSize; ++i)
				for (int j = 0; j < cellSize; ++j)
					vram[(Y * cellSize + i) * windowWidth + (X * cellSize + j)] = Color;
		}

		void Draw() const
		{
			for (int j = 0; j < m_Height; ++j)
			{
				for (int i = 0; i < m_Width; ++i)
				{
					BlockType type = m_Storage2D(i,j);
					cout << m_StorageTextRenderer[type];
					drawCell(i, j, m_StorageColorFiller[type]);
				}
				cout << endl;
			}
		}
	};

	Storage m_Storage;

	void terminate() {	}
public :
	void initialize(int Width, int Height)
	{
		srand(static_cast<unsigned int>(time(NULL)));

		m_Storage.Create(Width, Height);
		m_Storage.Init();
	}
private:
	void reset()
	{
		m_Storage.Init();
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
			draw();

			bool bQuit=update();
			if (bQuit)
				break;
		}
	}
public :
	bool update()
	{
		char input = getInput();
		if (input == 'q')
			return true;
		else if (m_Storage.IsComplete())
		{
			if (input == 'y')
			{
				reset();
				return false;
			}
			else
				return true;
		}

		MoveAction move = GetMoveAction(input);
		m_Storage.PlayerMove(move);
		return false;
	}
private:
	char getInput()
	{
		char inputChar = 0;
		cin >> inputChar;
		return inputChar;
	}

	MoveAction GetMoveAction(char input)
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
			cout << "Undefined input:" << input << endl;
		}

		return result;
	}

	void drawGameInfo()
	{
		cout << "Player Move Count : "<<m_Storage.GetPlayerMoveCnt()<<endl;
	}
public :
	void draw()
	{
		clearScreen();
		drawGameInfo();
		m_Storage.Draw();
		cout << endl;
		drawDebugInfo();

		if (m_Storage.IsComplete())
			drawWinMessage();
		else
			drawInputMessage();
	}
private:
	void drawDebugInfo()
	{
		drawPlayerIndex();
		drawStorageInfo();
		drawPrevFrameLog();
	}

	void drawPrevFrameLog()
	{
		cout <<endl<< "PrevFrameLog : " << m_Storage.GetPrevFrameLog()<< endl;
		m_Storage.ClearLog();
	}

	void drawStorageInfo() const
	{
		cout << "StorageWidth : " << m_Storage.GetWidth() << ", StorageHeight:" << m_Storage.GetHeight() << ",  BoxCnt:"<<m_Storage.GetBoxCnt() << endl;
	}

	void drawPlayerIndex() const
	{
		cout << "플레이어Index:" << m_Storage.GetPlayerIndex() << endl;
	}

	void clearScreen() const
	{
		system("cls");
	}

public:
	Game() { }
	void Start(int width, int height)
	{
		initialize(width, height);
		gameLoop();
		terminate();
	}

};

//#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
//int main()
//{
////	cout.imbue(locale("kor"));
//	Game game;
//	game.Start(8,8);
//	return 0;
//}

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
namespace GameLib
{
	void Framework::update()
	{
		static Game game;
		static bool bInit = false;
		if (!bInit)
		{
			game.initialize(8, 10);
			game.draw();
			bInit = true;
		}
		else
		{
			bool bQuit=game.update();
			game.draw();
			if (bQuit)
				Framework::instance().requestEnd();
			if (Framework::instance().isEndRequested())
			{
				//종료처리가 필요하다면 여기다가 해도댐
			}
		}
	}
}


