#include "pch.h"

#include "maze_view.h"

#include "maze.h"
#include "sdl_helpers.h"

const static std::string kTitle = "Mein Maze";

MazeView::MazeView(Maze* pMaze) 
	: View(kTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kWindowWidth, kWindowHeight)
	, m_pMaze(pMaze) {
}

MazeView::MazeView(int32_t x, int32_t y, Maze* pMaze) 
	: View(kTitle, x, y, kWindowWidth, kWindowHeight)
	, m_pMaze(pMaze) {
}

void MazeView::HandleEvent(SDL_Event& event) {
	View::HandleEvent(event);

	if (event.window.windowID != SDL_GetWindowID(GetWindow())) {
		return;
	}

	if (event.type == SDL_MOUSEMOTION) {
		m_mousePosition.x = event.motion.x;
		m_mousePosition.y = event.motion.y;
	}

	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			m_highlightColor = { 0, 192, 0 };
		} else if (event.button.button == SDL_BUTTON_RIGHT) {
		}
	}

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			m_highlightColor = { 0, 128, 0 };
		} else if (event.button.button == SDL_BUTTON_RIGHT) {
		}
	}
}

void MazeView::Update() {

}

void MazeView::Render() {
	SDL_Renderer* pRenderer = GetRenderer();
	SDL_SetRenderDrawColor(pRenderer, 128, 128, 255, 255);
	SDL_RenderClear(pRenderer);

	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

	const int32_t numLines = kGridSize + 1;
	const int32_t cellSize = GetCellSize();

	for (int32_t l = 0; l < numLines; l++) {
		int32_t lineOffset = l * cellSize;
		SDL_RenderDrawLine(pRenderer, lineOffset, 0, lineOffset, kWindowHeight);
		SDL_RenderDrawLine(pRenderer, 0, lineOffset, kWindowHeight, lineOffset);
	}

	const Grid* pGrid = m_pMaze->GetGrid();
	for (int32_t y = 0; y < kGridSize; y++) {
		for (int32_t x = 0; x < kGridSize; x++) {
			if (pGrid->GetState(x, y) == GridState::Collision) {
				SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
				SDL_Rect rect = { x * cellSize + 1, y * cellSize + 1, cellSize - 1, cellSize - 1 };
				SDL_RenderFillRect(pRenderer, &rect);
			}
		}
	}

	if (m_showPath) {
		const std::vector<Int2>& path = m_pMaze->GetPath();
		for (auto pos : path) {
			SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
			SDL_Rect rect = { pos.x * cellSize + 1, pos.y * cellSize + 1, cellSize - 1, cellSize - 1 };
			SDL_RenderFillRect(pRenderer, &rect);
		}
	}

	if (m_showPlayer) {
		SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
		SDL_RenderFillTriangle(pRenderer, m_playerPosition.x, m_playerPosition.y, kWindowWidth, 0, kWindowWidth - 50, kWindowHeight - 50);

		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
		SDL_RenderFillCircle(pRenderer, m_playerPosition.x, m_playerPosition.y, 16);

		const float distance = 32.0f;
		const float x = distance * cosf(m_playerAngle);
		const float y = distance * sinf(m_playerAngle);
		SDL_RenderDrawLine(pRenderer, m_playerPosition.x, m_playerPosition.y, m_playerPosition.x + x, m_playerPosition.y + y);
	}

	const int32_t x = ((m_mousePosition.x - 2) / cellSize) * cellSize;
	const int32_t y = ((m_mousePosition.y - 3) / cellSize) * cellSize;
	SDL_SetRenderDrawColor(pRenderer, m_highlightColor.x, m_highlightColor.y, m_highlightColor.z, 255);
	const SDL_Rect rect = {x + 1, y + 1, cellSize - 1, cellSize - 1};
	SDL_RenderFillRect(pRenderer, &rect);
	SDL_RenderPresent(pRenderer);
}
