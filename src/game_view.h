#pragma once

#include "view.h"

class GameView : public View{
public:
	GameView(int32_t x, int32_t y);

protected:
	virtual void HandleEvent(SDL_Event& event) override;

	virtual void Update();
	virtual void Render();
};
