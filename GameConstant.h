#pragma once

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

class Object
{
	static int m_MoveCount;
	BlockType m_Type = EMPTY_SLOT;
	int m_MoveX = 0;
	int m_MoveY = 0;
public:
	static int MoveCount() { return m_MoveCount; }
	static void SetMoveCount(int MoveCount) { m_MoveCount = MoveCount; }

	int MoveX() const { return m_MoveX; }
	int MoveY() const { return m_MoveY; }
	void SetMoveXY(int MoveX, int MoveY) { m_MoveX = MoveX; m_MoveY = MoveY; }
	void SetType(BlockType Type) { m_Type = Type; };
	BlockType GetType() const { return m_Type; }
	bool IsType(BlockType Type) const { return m_Type == Type; }
	bool IsWall() const { return m_Type == WALL; }
	bool IsBoxOnTheGoal() const { return m_Type == BOX_ON_THE_GOAL; }
	bool IsEmpty() const { return m_Type == EMPTY_SLOT; }
	bool IsGoal() const { return m_Type == GOAL; }
	bool HasWall() const { return m_Type & WALL; }
	bool HasBox() const { return m_Type & BOX; }
	bool HasGoal() const { return m_Type & GOAL; }
	bool HasPlayer() const { return m_Type & PLAYER; }
	void ReverseBox() { m_Type = static_cast<BlockType>(m_Type ^ BOX); }
	void ReversePlayer() { m_Type = static_cast<BlockType>(m_Type ^ PLAYER); }
};
int Object::m_MoveCount;