#pragma once

class Storage
{
	enum MoveAction
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		HOLD
	};

	Array2D<BlockType> m_Storage2D{ 8,8 };

	int m_Width = 0;
	int m_Height = 0;
	int m_BoxCnt = 0;

	Player m_Player;

	ostringstream	m_PrevFrameLog;

	void createWall()
	{
		for (int j = 0; j < m_Height; ++j)
		{
			for (int i = 0; i < m_Width; ++i)
			{
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
		if (m_Storage2D(X - 1, Y) != WALL && m_Storage2D(X + 1, Y) != WALL && m_Storage2D(X, Y - 1) != WALL && m_Storage2D(X, Y + 1) != WALL)
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

	void initPlayerPos()
	{
		while (true)
		{
			int x = (rand() % (m_Width - 2)) + 1;
			int y = (rand() % (m_Height - 2)) + 1;
			if (m_Storage2D(x, y) == EMPTY_SLOT)
			{
				m_Storage2D(x, y) = PLAYER;
				m_Player.SetPos(x, y);
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

	MoveAction getMoveAction(char input)
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
			std::cout << "Undefined input:" << input << endl;
		}

		return result;
	}

	void playerMove(MoveAction Move)
	{
		int dx = 0;
		int dy = 0;
		switch (Move)
		{
		case LEFT:
			dx = -1;
			break;
		case RIGHT:
			dx = 1;
			break;
		case UP:
			dy = -1;
			break;
		case DOWN:
			dy = 1;
			break;
		}

		int srcX = m_Player.PosX();
		int srcY = m_Player.PosY();
		int destX = srcX + dx;
		int destY = srcY + dy;

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
				m_Storage2D(destX, destY) = static_cast<BlockType>(m_Storage2D(destX, destY) ^ BOX);
				m_Storage2D(destX, destY) = static_cast<BlockType>(m_Storage2D(destX, destY) ^ PLAYER);
				m_Storage2D(srcX, srcY) = static_cast<BlockType>(m_Storage2D(srcX, srcY) ^ PLAYER);
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

public:
	Storage() {}
	virtual ~Storage() {}

	int Width() const { return m_Width; }
	int Height() const { return m_Height; }
	int PlayerPosX() const { return m_Player.PosX(); }
	int PlayerPosY() const { return m_Player.PosY(); }
	int BoxCnt() const { return m_BoxCnt; }
	int PlayerMoveCnt() const { return m_Player.MoveCnt(); }
	string PrevFrameLog() const { return m_PrevFrameLog.str(); }
	void ClearLog() { m_PrevFrameLog.str(""); m_PrevFrameLog << endl;; }

	void Create(int Width, int Height)
	{
		m_Width = Width;
		m_Height = Height;
		m_Storage2D.SetSize(Width, Height);
	}

	void Init()
	{
		m_Storage2D.Fill(EMPTY_SLOT);
		m_BoxCnt = 0;
		createWall();
		loadBox();
		loadGoal();

		initPlayerPos();
		m_Player.ResetMoveCnt();
	}

	void Update(char Input)
	{
		MoveAction move = getMoveAction(Input);
		playerMove(move);
	}

	bool IsComplete() const
	{
		int matchCnt = 0;
		for (int j = 0; j < m_Height; ++j)
		{
			for (int i = 0; i < m_Width; ++i)
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

	void Render(RenderInterface* RI) const
	{
		for (int j = 0; j < m_Height; ++j)
		{
			for (int i = 0; i < m_Width; ++i)
			{
				BlockType type = m_Storage2D(i, j);
				RI->Render(i, j, type);
			}
			std::cout << endl;
		}
	}
};
