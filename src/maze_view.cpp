#include "pch.h"

#include "maze_view.h"

#include "maze.h"
#include "sdl_helpers.h"

const static std::string kTitle = "Mein Maze";

const static float kFOV = 60.0f;
const static Float2 kProjectionPlane = { 320.0f, 200.0f };

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
	const int32_t tileSize = GetTileSize();

	for (int32_t l = 0; l < numLines; l++) {
		int32_t lineOffset = l * tileSize;
		SDL_RenderDrawLine(pRenderer, lineOffset, 0, lineOffset, kWindowHeight);
		SDL_RenderDrawLine(pRenderer, 0, lineOffset, kWindowHeight, lineOffset);
	}

	const Grid* pGrid = m_pMaze->GetGrid();
	for (int32_t y = 0; y < kGridSize; y++) {
		for (int32_t x = 0; x < kGridSize; x++) {
			if (pGrid->GetState(x, y) == GridState::Collision) {
				SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
				SDL_Rect rect = { x * tileSize + 1, y * tileSize + 1, tileSize - 1, tileSize - 1 };
				SDL_RenderFillRect(pRenderer, &rect);
			}
		}
	}

	if (m_showPath) {
		const std::vector<Int2>& path = m_pMaze->GetPath();
		for (auto pos : path) {
			SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
			SDL_Rect rect = { pos.x * tileSize + 1, pos.y * tileSize + 1, tileSize - 1, tileSize - 1 };
			SDL_RenderFillRect(pRenderer, &rect);
		}
	}

	if (m_showPlayer) {
		RenderRaycaster(pRenderer, m_pMaze);

		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
		SDL_RenderFillCircle(pRenderer, m_playerPosition.x, m_playerPosition.y, 16);

		float distance = 32.0f;
		const float x = m_playerPosition.x + distance * cosf(m_playerAngle);
		const float y = m_playerPosition.y + distance * sinf(m_playerAngle);
		SDL_RenderDrawLine(pRenderer, m_playerPosition.x, m_playerPosition.y, x, y);
	}

	const int32_t x = ((m_mousePosition.x - 2) / tileSize) * tileSize;
	const int32_t y = ((m_mousePosition.y - 3) / tileSize) * tileSize;
	SDL_SetRenderDrawColor(pRenderer, m_highlightColor.x, m_highlightColor.y, m_highlightColor.z, 255);
	const SDL_Rect rect = {x + 1, y + 1, tileSize - 1, tileSize - 1};
	SDL_RenderFillRect(pRenderer, &rect);
	SDL_RenderPresent(pRenderer);
}

void MazeView::RenderRaycaster(SDL_Renderer* pRenderer, const Maze* pMaze) {
	const float kMapHeight = kWindowHeight;
	const float kMapWidth = kWindowHeight;
	const float kTileSize = kMapHeight / kGridSize;
	const float kDepth = sqrtf((kWindowHeight * kWindowHeight) + (kWindowHeight * kWindowHeight));

	const Grid* pGrid = pMaze->GetGrid();

	float startAngle = Rad2Deg(m_playerAngle) - kFOV * 0.5f;
	for (int32_t x = 0; x < kProjectionPlane.x; x++) {
		const float rayAngle = Deg2Rad(startAngle + (kFOV / kProjectionPlane.x) * x);
		const Float2 eye = { cosf(rayAngle), sinf(rayAngle) };

		float distanceToWall = 0.0f;
		bool hitWall = false;
		bool boundry = false;
		while (!hitWall && distanceToWall < kDepth) {
			distanceToWall += 0.1f;

			Int2 test = { ((int32_t)(m_playerPosition.x + (eye.x * distanceToWall))) / (int32_t)kTileSize,
						  ((int32_t)(m_playerPosition.y + (eye.y * distanceToWall))) / (int32_t)kTileSize };

			if (test.x < 0 || test.x >= kGridSize || test.y < 0 || test.y >= kGridSize) {
				hitWall = true;
				break;
			} else {
				if (pGrid->GetState(test) == GridState::Collision) {
					hitWall = true;
					break;
				}
			}
		}

		if (hitWall) {
			const Float2 hit = { m_playerPosition.x + (eye.x * distanceToWall), m_playerPosition.y + (eye.y * distanceToWall) };
			SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(pRenderer, m_playerPosition.x, m_playerPosition.y, hit.x, hit.y);
		}
	}
}
