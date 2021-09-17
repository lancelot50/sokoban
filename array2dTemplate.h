#pragma once

template<class T>
class Array2D
{
	T* m_Array = nullptr;
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

public:
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

