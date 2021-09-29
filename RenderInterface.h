#pragma once

class RenderInterface
{
public:
	virtual void Render(int X, int Y, Object Obj) = 0;
};
