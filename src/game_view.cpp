#include "pch.h"

#include "game_view.h"

GameView::GameView() 
	: View(kTitle, 720, 405) {
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
