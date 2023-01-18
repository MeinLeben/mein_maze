#include "pch.h"

#include "maze_view.h"

#include "maze.h"
#include "sdl_helpers.h"

const static std::string kTitle = "Mein Maze";

MazeView::MazeView(int32_t width, int32_t height, Maze* pMaze) 
	: IView(kTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height)
	, m_pMaze(pMaze) {
	RayCaster::Get().RegisterHandler(this);
}

MazeView::MazeView(int32_t x, int32_t y, int32_t width, int32_t height, Maze* pMaze) 
	: IView(kTitle, x, y, width, height)
	, m_pMaze(pMaze) {
	RayCaster::Get().RegisterHandler(this);
}

MazeView::~MazeView() {
	RayCaster::Get().DeRegisterHandler(this);
}

void MazeView::HandleEvent(SDL_Event& event) {
	IView::HandleEvent(event);

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

void MazeView::PreRender() {
	SDL_Renderer* pRenderer = GetRenderer();
	SDL_SetRenderDrawColor(pRenderer, 128, 128, 255, 255);
	SDL_RenderClear(pRenderer);
}

void MazeView::Render() {
	SDL_Renderer* pRenderer = GetRenderer();
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

	for (int32_t l = 0; l < m_pMaze->GetNumTiles().x; l++) {
		int32_t lineOffset = l * m_pMaze->GetSizePerTile().x;
		SDL_RenderDrawLine(pRenderer, 0, lineOffset, GetSize().x, lineOffset);
	}

	for (int32_t l = 0; l < m_pMaze->GetNumTiles().y; l++) {
		int32_t lineOffset = l * m_pMaze->GetSizePerTile().y;
		SDL_RenderDrawLine(pRenderer, lineOffset, 0, lineOffset, GetSize().y);
	}
	
	const Grid* pGrid = m_pMaze->GetGrid();
	for (int32_t y = 0; y < m_pMaze->GetNumTiles().y; y++) {
		for (int32_t x = 0; x < m_pMaze->GetNumTiles().x; x++) {
			if (pGrid->GetState(x, y) == GridState::Collision) {
				SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
				SDL_Rect rect = { x * m_pMaze->GetSizePerTile().x + 1, y * m_pMaze->GetSizePerTile().y + 1, m_pMaze->GetSizePerTile().x - 1, m_pMaze->GetSizePerTile().y - 1 };
				SDL_RenderFillRect(pRenderer, &rect);
			}
		}
	}

	if (m_showPath) {
		const std::vector<Int2>& path = m_pMaze->GetPath();
		for (auto pos : path) {
			SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
			SDL_Rect rect = { pos.x * m_pMaze->GetSizePerTile().x + 1, pos.y * m_pMaze->GetSizePerTile().y + 1, m_pMaze->GetSizePerTile().x - 1, m_pMaze->GetSizePerTile().y - 1 };
			SDL_RenderFillRect(pRenderer, &rect);
		}
	}

	if (m_showPlayer) {
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
		SDL_RenderFillCircle(pRenderer, m_playerPosition.x, m_playerPosition.y, 16);

		float distance = 32.0f;
		const float x = m_playerPosition.x + distance * cosf(m_playerAngle);
		const float y = m_playerPosition.y + distance * sinf(m_playerAngle);
		SDL_RenderDrawLine(pRenderer, m_playerPosition.x, m_playerPosition.y, x, y);
	}

	const int32_t x = ((m_mousePosition.x - 2) / m_pMaze->GetSizePerTile().x) * m_pMaze->GetSizePerTile().x;
	const int32_t y = ((m_mousePosition.y - 3) / m_pMaze->GetSizePerTile().y) * m_pMaze->GetSizePerTile().y;
	SDL_SetRenderDrawColor(pRenderer, m_highlightColor.x, m_highlightColor.y, m_highlightColor.z, 255);
	const SDL_Rect rect = {x + 1, y + 1, m_pMaze->GetSizePerTile().x - 1, m_pMaze->GetSizePerTile().y - 1};
	SDL_RenderFillRect(pRenderer, &rect);
	SDL_RenderPresent(pRenderer);
}

void MazeView::OnHit(const Ray& ray) {
	if (m_showPlayer) {
		const Float2 hit = { ray.startPosition.x + (ray.direction.x * ray.distanceTo), ray.startPosition.y + (ray.direction.y * ray.distanceTo) };
		SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 0, 255);
		SDL_RenderDrawLine(GetRenderer(), ray.startPosition.x, ray.startPosition.y, hit.x, hit.y);
	}
}
