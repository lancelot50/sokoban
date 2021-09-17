#pragma once

class D3DRenderInterface : public RenderInterface
{
	unsigned int m_StorageColorFiller[MAX_BLOCK_TYPE] = { EMPTY_SLOT, };

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

	void drawCell(int X, int Y, unsigned int Color) const
	{
		unsigned* vram = Framework::instance().videoMemory();
		unsigned windowWidth = Framework::instance().width();
		static const int cellSize = 16;

		for (int i = 0; i < cellSize; ++i)
			for (int j = 0; j < cellSize; ++j)
				vram[(Y * cellSize + i) * windowWidth + (X * cellSize + j)] = Color;
	}

public:
	D3DRenderInterface()
	{
		createStorageColorFiller();
	}
	void Render(int X, int Y, BlockType Type)
	{
		drawCell(X, Y, m_StorageColorFiller[Type]);
	}
};
