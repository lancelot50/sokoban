#pragma once

class ConsoleRenderInterface : public RenderInterface
{
	char m_StorageTextRenderer[MAX_BLOCK_TYPE] = { EMPTY_SLOT, };

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

public:
	ConsoleRenderInterface()
	{
		createBlockRenderArray();
	}
	void Render(int X, int Y, Object Obj)
	{
		std::cout << m_StorageTextRenderer[Obj.GetType()];
	}
};
