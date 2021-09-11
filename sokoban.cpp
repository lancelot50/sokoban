#include<iostream>
#include<string>
using namespace std;
#include<stdlib.h>

#include<locale>
#include <assert.h>

#include<sstream>

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
		};

		wchar_t m_BlockRenderArray[PLAYER_ON_THE_GOAL] = { EMPTY_SLOT, };

		class Player
		{
			int m_PosIdx;
			int m_MoveCnt;
		public:
			Player() :m_PosIdx(0), m_MoveCnt(0) {}
			void SetPos(int PosIdx)
			{
				m_PosIdx = PosIdx;
				++m_MoveCnt;
			}
			int GetPos() const { return m_PosIdx; }
			int GetMoveCnt() const { return m_MoveCnt; }
			void ResetMoveCnt() { m_MoveCnt = 0; }
		};

		BlockType* m_StorageArray;

		int m_Width;
		int m_Height;
		int m_Size;
		int m_BoxCnt;

		Player m_Player;

		wostringstream	m_PrevFrameLog;

		void createWall()
		{
			for (int i = 0; i < m_Width; ++i)
			{
				for (int j = 0; j < m_Height; ++j)
				{
					assert(i * m_Height + j < m_Size);		// code analysis 오류나서

					if (i == 0 || j == 0 || j == m_Height - 1 || i == m_Width - 1)
						m_StorageArray[i * m_Height + j] = WALL;
					else
						m_StorageArray[i * m_Height + j] = EMPTY_SLOT;
				}
			}
		}

		bool isEdge(int Idx) const
		{
			if (m_StorageArray[Idx - 1] != WALL && m_StorageArray[Idx + 1] != WALL && m_StorageArray[Idx - m_Width] != WALL && m_StorageArray[Idx + m_Width] != WALL)
				return false;
			else
				return true;
		}

		void loadBox()
		{
			while (m_BoxCnt <= 0)
			{
				for (int i = 0; i < m_Width; ++i)
				{
					for (int j = 0; j < m_Height; ++j)
					{
						int curIndex = i * m_Height + j;
						assert(curIndex < m_Size);		// code analysis 오류나서
						if (m_StorageArray[curIndex] != WALL && rand() % 10 == 0 && !isEdge(curIndex))
						{
							m_StorageArray[curIndex] = BOX;
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
				int index = rand() % m_Size;
				if (m_StorageArray[index] == EMPTY_SLOT)
				{
					m_StorageArray[index] = GOAL;
					--boxCnt;
				}
			}
		}

	public:
		Storage() : m_Width(0), m_Height(0), m_Size(0), m_StorageArray(0), m_BoxCnt(0) {}
		virtual ~Storage()
		{
			delete[] m_StorageArray;
		}

		int GetSize() const { return m_Size; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetPlayerIndex() const { return m_Player.GetPos(); }
		int GetBoxCnt() const { return m_BoxCnt; }
		int GetPlayerMoveCnt() const { return m_Player.GetMoveCnt(); }
		wstring GetPrevFrameLog() const { return m_PrevFrameLog.str(); }
		void ClearLog() { m_PrevFrameLog.str(L""); m_PrevFrameLog << endl;; }

		void createBlockRenderArray()
		{
			m_BlockRenderArray[EMPTY_SLOT] = L' ';
			m_BlockRenderArray[WALL] = L'|';
			m_BlockRenderArray[GOAL] = L'.';
			m_BlockRenderArray[BOX] = L'o';
			m_BlockRenderArray[PLAYER] = L'p';
			m_BlockRenderArray[BOX_ON_THE_GOAL] = L'O';
			m_BlockRenderArray[PLAYER_ON_THE_GOAL] = L'P';
		}

		void Create(int Width, int Height)
		{
			m_Width = Width;
			m_Height = Height;
			m_Size = Width * Height;
			m_StorageArray = new BlockType[m_Size];
			createBlockRenderArray();
		}

		void Init()
		{
			for (int i = 0; i < m_Size; ++i)
				m_StorageArray[i] = EMPTY_SLOT;
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
				int pos = rand() % m_Size;
				if (m_StorageArray[pos] == EMPTY_SLOT)
				{
					m_StorageArray[pos] = PLAYER;
					m_Player.SetPos(pos);
					break;
				}
			}
		}

		bool IsMovableBlock(int DestIndex)
		{
			bool ret = false;

			m_PrevFrameLog << L"IsMovableBlock(" << DestIndex << L")";
			m_PrevFrameLog.setf(ios_base::hex, ios_base::basefield);
			m_PrevFrameLog.setf(ios_base::showbase);
			m_PrevFrameLog << L", Block:" << m_StorageArray[DestIndex] << endl;;
			m_PrevFrameLog.unsetf(ios_base::hex);

			if (m_StorageArray[DestIndex] & WALL)
			{
				m_PrevFrameLog << L"WALL에 막힘" << endl;
				ret = false;
			}
			else
			{
				ret = true;
			}
			return ret;
		}

		bool IsValidIndex(int PlayerDestIndex)
		{
			if (PlayerDestIndex > 0 && PlayerDestIndex < m_Size)
			{
				return true;
			}
			else
			{
				m_PrevFrameLog << L"InvalidIndex:" << PlayerDestIndex << endl;
				return false;
			}
		}

		bool blockHasPlayer(int Index) const
		{
			if (m_StorageArray[Index] & PLAYER)
				return true;
			else
				return false;
		}

		bool blockHasBox(int Index) const
		{
			if (m_StorageArray[Index] & BOX)
				return true;
			else
				return false;
		}

		bool processMove(int SrcIndex, int DestIndex)
		{
			m_PrevFrameLog << L"processMove(" << SrcIndex << L", " << DestIndex << L")" << endl;

			if (!IsValidIndex(SrcIndex))
				return false;

			bool ret = false;

			if (IsMovableBlock(DestIndex))
			{
				if (blockHasPlayer(SrcIndex) && blockHasBox(DestIndex))
				{
					int srcIdx = DestIndex;
					int destIdx = DestIndex + (DestIndex - SrcIndex);
					bool result = processMove(srcIdx, destIdx);

					if (result)
					{
						if (blockHasPlayer(SrcIndex))
						{
							m_StorageArray[SrcIndex] = static_cast<BlockType>(m_StorageArray[SrcIndex] ^ PLAYER);
							m_StorageArray[DestIndex] = static_cast<BlockType>(m_StorageArray[DestIndex] ^ PLAYER);

							if (blockHasPlayer(DestIndex))
								m_Player.SetPos(DestIndex);
						}
					}
				}
				else
				{
					if (blockHasPlayer(SrcIndex))
					{
						m_StorageArray[SrcIndex] = static_cast<BlockType>(m_StorageArray[SrcIndex] ^ PLAYER);
						m_StorageArray[DestIndex] = static_cast<BlockType>(m_StorageArray[DestIndex] ^ PLAYER);

						if (blockHasPlayer(DestIndex))
							m_Player.SetPos(DestIndex);
					}
					else if (blockHasBox(SrcIndex) && !blockHasBox(DestIndex))
					{
						m_StorageArray[SrcIndex] = static_cast<BlockType>(m_StorageArray[SrcIndex] ^ BOX);
						m_StorageArray[DestIndex] = static_cast<BlockType>(m_StorageArray[DestIndex] ^ BOX);
						ret = true;
					}
				}
			}

			return ret;
		}

		void PlayerMoveLeft()
		{
			// 1. 플레이어가 왼쪽으로 이동가능한가?
			//		왼쪽에 박스가 있나?
			//			박스가 왼쪽으로 이동가능한가?
			//				가능하다면 박스를 이동
			//				플레이어도 이동
			//				불가능하다면 박스이동불가
			//				플레이어도 이동불가.
			int playerLeftIndex = m_Player.GetPos() - 1;

			int srcIndex = m_Player.GetPos();
			int destIndex = playerLeftIndex;

			processMove(srcIndex, destIndex);
		}

		void PlayerMoveRight()
		{
			int playerRightIndex = m_Player.GetPos() + 1;

			int srcIndex = m_Player.GetPos();
			int destIndex = playerRightIndex;

			processMove(srcIndex, destIndex);
		}

		void PlayerMoveUp()
		{
			int playerUpIndex = m_Player.GetPos() - m_Height;

			int srcIndex = m_Player.GetPos();
			int destIndex = playerUpIndex;

			processMove(srcIndex, destIndex);
		}
		void PlayerMoveDown()
		{
			int playerDownIndex = m_Player.GetPos() + m_Height;

			int srcIndex = m_Player.GetPos();
			int destIndex = playerDownIndex;

			processMove(srcIndex, destIndex);
		}

		bool IsComplete() const
		{
			int matchCnt = 0;
			for (int i = 0; i < m_Size; ++i)
			{
				if (m_StorageArray[i] == BOX_ON_THE_GOAL)
					++matchCnt;
			}

			if (matchCnt == m_BoxCnt)
				return true;
			else
				return false;
		}

		void Draw() const
		{
			for (int i = 0; i < m_Width; ++i)
			{
				for (int j = 0; j < m_Height; ++j)
					wcout << m_BlockRenderArray[m_StorageArray[i * m_Height + j]];
				wcout << endl;
			}
		}
	};

	Storage m_Storage;

	void terminate() {	}
	void initialize(int Width, int Height)
	{
		srand(static_cast<unsigned int>(time(NULL)));

		m_Storage.Create(Width, Height);
		m_Storage.Init();
	}

	void reset()
	{
		m_Storage.Init();
	}

	void drawWinMessage()
	{
		wcout << L"******************" << endl;
		wcout << L"     You Win!" << endl;
		wcout << L"******************" << endl;
		wcout << L"**** continue(y/n)? ***" << endl;
	}
	void drawInputMessage()
	{
		wcout << L"*************************************" << endl;
		wcout << L"LEFT:a, RIGHT:d, UP:w, DOWN:x, QUIT:q" << endl;
	}

	void gameLoop()
	{
		while (true)
		{
			draw();
			if (m_Storage.IsComplete())
				drawWinMessage();
			else
				drawInputMessage();

			bool bQuit=update();
			if (bQuit)
				break;
		}
	}

	bool update()
	{
		wchar_t input = getInput();
		if (input == L'q')
			return true;
		else if (m_Storage.IsComplete())
		{
			if (input == L'y')
			{
				reset();
				return false;
			}
			else
				return true;
		}

		MoveAction move = GetMoveAction(input);
		movePlayerPosition(move);
		return false;
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
		}

		return result;
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
	
	void drawGameInfo()
	{
		wcout << "Player Move Count : "<<m_Storage.GetPlayerMoveCnt()<<endl;
	}

	void draw()
	{
		clearScreen();
		drawGameInfo();
		m_Storage.Draw();
		wcout << endl;
		drawDebugInfo();
	}

	void drawDebugInfo()
	{
		drawPlayerIndex();
		drawStorageInfo();
		drawPrevFrameLog();
	}

	void drawPrevFrameLog()
	{
		wcout <<endl<< L"PrevFrameLog : " << m_Storage.GetPrevFrameLog()<< endl;
		m_Storage.ClearLog();
	}

	void drawStorageInfo() const
	{
		wcout << L"StorageWidth : " << m_Storage.GetWidth() << L", StorageHeight:" << m_Storage.GetHeight() << ",  BoxCnt:"<<m_Storage.GetBoxCnt() << endl;
	}

	void drawPlayerIndex() const
	{
		wcout << L"플레이어Index:" << m_Storage.GetPlayerIndex() << endl;
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

int main()
{
	wcout.imbue(locale("kor"));

	Game game;
	game.Start(8,8);
	return 0;
}