#include "pch.h"

#include "game_view.h"

const static std::string kTitle = "Mein Game";

const static int32_t kWindowWidth = 720;
const static int32_t kWindowHeight = 405;

GameView::GameView(int32_t x, int32_t y) 
	: View(kTitle, x, y, kWindowWidth, kWindowHeight) {
}

void GameView::HandleEvent(SDL_Event& event) {
	View::HandleEvent(event);
}

void GameView::Update() {

}

void GameView::Render() {
	SDL_SetRenderDrawColor(GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(GetRenderer());

	SDL_RenderPresent(GetRenderer());
}
