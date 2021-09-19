#pragma once

#include<fstream>
using namespace std;
class File
{
	int m_Size=0;
	char* m_Data = nullptr;
public :
	File(const char* filename)
	{
		ifstream inFile(filename, ifstream::binary);
		if (inFile)
		{
			inFile.seekg(0, ifstream::end);				// 맨끝으로 이동
			m_Size = static_cast<int>(inFile.tellg());	// 맨끝의 위치== 파일크기
			m_Data = new char[m_Size];					// 크기만큼 공간 확보
			inFile.seekg(0, ifstream::beg);				// 처음으로 이동
			inFile.read(m_Data, m_Size);				// 읽기
		}
	}
	~File()
	{
		delete[] m_Data;
		m_Data = nullptr;
	}
	int Size() const { return m_Size; }
	char* Data() const { return m_Data; }
	unsigned int GetUnsigned(int Pos) const
	{
		const unsigned char* up;
		up = reinterpret_cast<unsigned char*>(m_Data);
		unsigned int r=up[Pos];
		r |= up[Pos + 1]<<8;
		r |= up[Pos + 2]<<16;
		r |= up[Pos + 3]<<24;
		return r;
	}
};


enum ImageID
{
	IMAGE_ID_PLAYER,
	IMAGE_ID_WALL,
	IMAGE_ID_BLOCK,
	IMAGE_ID_GOAL,
	IMAGE_ID_SPACE,
};

class Image
{
	int m_Width=0;
	int m_Height=0;
	unsigned int* m_Data=nullptr;
public:
	Image(const char* filename)
	{
		File f(filename);
		m_Height = f.GetUnsigned(12);
		m_Width = f.GetUnsigned(16);
		int size = m_Width * m_Height;
		m_Data = new unsigned int[size];
		for (int i = 0; i < size; ++i)
			m_Data[i] = f.GetUnsigned(128 + i * 4);
	}
	~Image()
	{
		delete[] m_Data;
		m_Data = nullptr;
	}
	int Width() const { return m_Width; }
	int Height() const { return m_Height; }
	unsigned int* Data() const { return m_Data; }
	void Draw(int x, int y, ImageID ID)
	{
		drawAlphaBlend(x * 32, y * 32, ID * 32, 0, 32, 32);
	}

	void draw(int DestX, int DestY, int SrcX, int SrcY, int Width, int Height) const
	{
		unsigned int* vram = Framework::instance().videoMemory();
		unsigned windowWidth = Framework::instance().width();

		for (int y = 0; y < Height; ++y)
			for (int x = 0; x < Width; ++x)
			{
				unsigned src = m_Data[(y + SrcY) * m_Width + (x + SrcX)];
				if (src & 0x80000000)
				{
					unsigned int* dest = &vram[(y + DestY) * windowWidth + (x + DestX)];
					*dest = src;
				}
			}
	}

	void drawAlphaBlend(int DestX, int DestY, int SrcX, int SrcY, int Width, int Height) const
	{
		unsigned int* vram = Framework::instance().videoMemory();
		unsigned windowWidth = Framework::instance().width();

		for (int y = 0; y < Height; ++y)
			for (int x = 0; x < Width; ++x)
			{
				unsigned src = m_Data[(y + SrcY) * m_Width + (x + SrcX)];
				unsigned int* dest = &vram[(y + DestY) * windowWidth + (x + DestX)];
				unsigned int srcA = (src & 0xff000000) >> 24;
				unsigned int srcR = src & 0xff0000;
				unsigned int srcG = src & 0x00ff00;
				unsigned int srcB = src & 0x0000ff;
				unsigned int destR = *dest & 0xff0000;
				unsigned int destG = *dest & 0x00ff00;
				unsigned int destB = *dest & 0x0000ff;
				unsigned int r = (srcR - destR) * srcA / 255 + destR;
				unsigned int g = (srcG - destG) * srcA / 255 + destG;
				unsigned int b = (srcB - destB) * srcA / 255 + destB;
				*dest = (r & 0xff0000) | (g & 0x00ff00) | b;
			}
	}


};

class D3DRenderInterface : public RenderInterface
{
	Image* m_pImage;

	unsigned int m_StorageColorFiller[MAX_BLOCK_TYPE] = { EMPTY_SLOT, };
	ImageID m_BlockTypeImageIDConverter[MAX_BLOCK_TYPE] = { IMAGE_ID_SPACE, };

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

	void createBlockTypeImageIDConverter()
	{
		m_BlockTypeImageIDConverter[EMPTY_SLOT] = IMAGE_ID_SPACE;
		m_BlockTypeImageIDConverter[WALL] = IMAGE_ID_WALL;
		m_BlockTypeImageIDConverter[GOAL] = IMAGE_ID_GOAL;
		m_BlockTypeImageIDConverter[BOX] = IMAGE_ID_BLOCK;
		m_BlockTypeImageIDConverter[PLAYER] = IMAGE_ID_PLAYER;
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
		createBlockTypeImageIDConverter();
		m_pImage = new Image("nimotsuKunImage2.dds");
	}
	~D3DRenderInterface()
	{
		delete m_pImage;
	}
	void Render(int X, int Y, BlockType Type)
	{
		if ((Type & WALL))
		{
			ImageID id = m_BlockTypeImageIDConverter[Type];
			m_pImage->Draw(X, Y, id);
		}
		else if( Type & GOAL)
			m_pImage->Draw(X, Y, m_BlockTypeImageIDConverter[GOAL]);
		else
			m_pImage->Draw(X, Y, m_BlockTypeImageIDConverter[EMPTY_SLOT]);

		if (Type & PLAYER)
			m_pImage->Draw(X, Y, IMAGE_ID_PLAYER);
		else if( Type & BOX)
			m_pImage->Draw(X, Y, IMAGE_ID_BLOCK);
	}
};
