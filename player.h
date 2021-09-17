#pragma once

class Player
{
	int m_MoveCnt = 0;
	int m_PosX = 0;
	int m_PosY = 0;
public:
	Player() {}
	void SetPos(int PosX, int PosY)
	{
		m_PosX = PosX;
		m_PosY = PosY;
		++m_MoveCnt;
	}
	int GetPosX() const { return m_PosX; }
	int GetPosY() const { return m_PosY; }
	int GetMoveCnt() const { return m_MoveCnt; }
	void ResetMoveCnt() { m_MoveCnt = 0; }
};
