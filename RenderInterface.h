#pragma once

class RenderInterface
{
public:
	virtual void Render(int X, int Y, BlockType Type) = 0;
};
