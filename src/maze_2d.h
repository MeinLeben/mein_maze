#pragma once

#include "view.h"

class Maze2d : public View {
public:
	Maze2d();

protected:
	virtual void HandleEvent(SDL_Event& event);

	virtual void Update();
	virtual void Render();
};