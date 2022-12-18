#include "pch.h"

#include "maze_2d.h"

Maze2d::Maze2d() : View("Maze2d", 720, 405) {

}

void Maze2d::HandleEvent(SDL_Event& event) {
}

void Maze2d::Update() {
}

void Maze2d::Render() {
	SDL_SetRenderDrawColor(GetRenderer(), 128, 128, 255, 255);
	SDL_RenderClear(GetRenderer());
	SDL_RenderPresent(GetRenderer());
}