#include "pch.h"

#include "game_view.h"

#include "time.h"

const static std::string kTitle = "Mein Game";

const static int32_t kWindowWidth = 720;
const static int32_t kWindowHeight = 405;

GameView::GameView(int32_t x, int32_t y) 
	: IView(kTitle, x, y, kWindowWidth, kWindowHeight) {
}

void GameView::HandleEvent(SDL_Event& event) {
	IView::HandleEvent(event);
}

void GameView::Update() {
}

void GameView::PreRender() {
	SDL_Renderer* pRenderer = GetRenderer();
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(pRenderer);
}

void GameView::Render() {
	SDL_Renderer* pRenderer = GetRenderer();
	SDL_RenderPresent(pRenderer);
}
