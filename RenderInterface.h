#pragma once

class RenderInterface
{
public:
	virtual void RenderBackground(int X, int Y, Object Obj) = 0;
	virtual void RenderForeground(int X, int Y, Object Obj) = 0;
};
