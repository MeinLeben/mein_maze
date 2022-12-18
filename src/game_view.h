#pragma once

#include "view.h"

class GameView : public View{
public:
	GameView();

protected:
	virtual void HandleEvent(SDL_Event& event) override;

	virtual void Update();
	virtual void Render();
};
