#pragma once

#include "view.h"

class GameView : public IView{
public:
	GameView(int32_t x, int32_t y);

protected:
	virtual void HandleEvent(SDL_Event& event) override;

	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
};
