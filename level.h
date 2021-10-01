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

	Array2D<Object> m_Storage2D{ 8,8 };

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
					m_Storage2D(i, j).SetType(WALL);
				}
				else
				{
					m_Storage2D(i, j).SetType(EMPTY_SLOT);
				}
			}
		}
	}

	bool isEdge(int X, int Y) const
	{
		if (!m_Storage2D(X - 1, Y).IsWall() && !m_Storage2D(X + 1, Y).IsWall() && !m_Storage2D(X, Y - 1).IsWall() && !m_Storage2D(X, Y + 1).IsWall())
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

					if ( !m_Storage2D(i, j).IsWall() && !isEdge(i, j) && bCreateBox)
					{
						m_Storage2D(i, j).SetType(BOX);
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

			if (m_Storage2D(x, y).IsEmpty())
			{
				m_Storage2D(x, y).SetType(GOAL);
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
			if (m_Storage2D(x, y).IsEmpty())
			{
				m_Storage2D(x, y).SetType(PLAYER);
				m_Player.SetPos(x, y);
				break;
			}
		}
	}

	void printBlock(int DestX, int DestY)
	{
		m_PrevFrameLog.setf(ios_base::hex, ios_base::basefield);
		m_PrevFrameLog.setf(ios_base::showbase);
		m_PrevFrameLog << ", dest Block:" << m_Storage2D(DestX, DestY).GetType() << endl;;
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
			m_PrevFrameLog << "Undefined input:" << input << endl;
		}

		return result;
	}

	void playerMove(MoveAction Move)
	{
		m_PrevFrameLog << Object::MoveCount() << endl;
		if (Object::MoveCount() == 32)
		{
			Object::SetMoveCount(0);
			for (int y = 0; y < m_Height; ++y)
				for (int x = 0; x < m_Width; ++x)
					m_Storage2D(x, y).SetMoveXY(0, 0);
		}
		if (Object::MoveCount() > 0)
		{
			Object::SetMoveCount(Object::MoveCount() + 1);
			return;
		}


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
		if (m_Storage2D(destX, destY).HasWall())
		{
			m_PrevFrameLog << "WALL에 막힘" << endl;
			return;
		}
		else if (m_Storage2D(destX, destY).HasBox())
		{
			m_PrevFrameLog << "앞에 BOX" << endl;
			int destDestX = destX + dx;
			int destDestY = destY + dy;
			bool isWall = m_Storage2D(destDestX, destDestY).HasWall();
			bool isBox = m_Storage2D(destDestX, destDestY).HasBox();
			if (!isWall && !isBox)
			{
				m_Storage2D(destDestX, destDestY).ReverseBox();
				m_Storage2D(destDestX, destDestY).SetMoveXY(dx, dy);
				m_Storage2D(destX, destY).ReverseBox();
				m_Storage2D(destX, destY).ReversePlayer();
				m_Storage2D(destX, destY).SetMoveXY(dx, dy);
				m_Storage2D(srcX, srcY).ReversePlayer();
				m_Storage2D(srcX, srcY).SetMoveXY(dx, dy);
				m_Player.SetPos(destX, destY);
				Object::SetMoveCount(1);
			}
		}
		else if(m_Storage2D(destX, destY).IsEmpty() || m_Storage2D(destX, destY).IsGoal())
		{
			m_Storage2D(destX, destY).ReversePlayer();
			m_Storage2D(destX, destY).SetMoveXY(dx, dy);
			m_Storage2D(srcX, srcY).ReversePlayer();
			m_Storage2D(srcX, srcY).SetMoveXY(dx, dy);
			m_Player.SetPos(destX, destY);
			Object::SetMoveCount(1);
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
				if (m_Storage2D(i, j).IsBoxOnTheGoal())
					++matchCnt;
			}
		}

		if (matchCnt == m_BoxCnt)
			return true;
		else
			return false;
	}

	void renderForeground(RenderInterface* RI) const
	{
		for (int j = 0; j < m_Height; ++j)
		{
			for (int i = 0; i < m_Width; ++i)
			{
				Object obj = m_Storage2D(i, j);
				RI->RenderForeground(i, j, obj);
			}
		}
	}
	void renderBackground(RenderInterface* RI) const
	{
		for (int j = 0; j < m_Height; ++j)
		{
			for (int i = 0; i < m_Width; ++i)
			{
				Object obj = m_Storage2D(i, j);
				RI->RenderBackground(i, j, obj);
			}
		}
	}

	void Render(RenderInterface* RI) const
	{
		renderBackground(RI);
		renderForeground(RI);
	}
};
