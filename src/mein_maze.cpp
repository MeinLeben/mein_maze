#include "pch.h"

#include "mein_maze.h"
#include "grid.h"

MeinMaze::MeinMaze() {
	std::srand(std::time(nullptr));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		throw std::runtime_error(SDL_GetError());
	}

	m_maze = std::make_unique<Maze>();

	m_window = std::make_unique<SDLWrapper::Window>("MeinMaze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kWindowWidth, kWindowHeight, 0);
	m_renderer = std::make_unique<SDLWrapper::Renderer>(m_window->get(), -1, SDL_RENDERER_ACCELERATED);
}

MeinMaze::~MeinMaze() {
	SDL_Quit();
}

int32_t MeinMaze::Run() {
	SDL_Event event = {};
	while (true) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return 0;
			}

			if (event.type == SDL_WINDOWEVENT) {
				switch (event.window.type) {
				case SDL_WINDOWEVENT_CLOSE:
					return 0;
				default:
					break;
				}
			}

			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					return 0;
				case SDL_SCANCODE_SPACE:
					m_auto = false;
					m_maze->GenerateRandomPattern(kMazeWeight);
					break;
				case SDL_SCANCODE_RETURN: {
					m_auto = false;
					m_maze->FindPath(m_start, m_destination);
				} break;
				case SDL_SCANCODE_TAB:
					m_auto = !m_auto;
					break;
				default:
					break;
				}
			}
		}

		Update();
		Render();
//		SDL_Delay(500);
	}

	return 0;
}

void MeinMaze::Update() {
	if (m_auto) {
		m_maze->GenerateRandomPattern(kMazeWeight);
		if (!m_maze->FindPath(m_start, m_destination)) {
			m_numberOfFails++;
			std::string title = "MeinMaze | Number of fails: " + std::to_string(m_numberOfFails);
			SDL_SetWindowTitle(m_window->get(), title.c_str());
		} else {
			m_numberOfFails = 0;
			m_auto = false;
		}
	}
}

void MeinMaze::Render() {
	SDL_SetRenderDrawColor(m_renderer->get(), 128, 128, 255, 255);
	SDL_RenderClear(m_renderer->get());

	SDL_SetRenderDrawColor(m_renderer->get(), 0, 0, 0, 255);

	const int32_t numLines = kGridSize + 1;
	const int32_t cellSize = kWindowHeight / kGridSize;

	for (int32_t l = 0; l < numLines; l++) {
		int32_t lineOffset = l * cellSize;
		SDL_RenderDrawLine(m_renderer->get(), lineOffset, 0, lineOffset, kWindowHeight);
		SDL_RenderDrawLine(m_renderer->get(), 0, lineOffset, kWindowHeight, lineOffset);
	}

	const Grid* pGrid = m_maze->GetGrid();
	for (int32_t y = 0; y < kGridSize; y++) {
		for (int32_t x = 0; x < kGridSize; x++) {
			if (pGrid->GetState(x, y) == GridState::Collision) {
				SDL_SetRenderDrawColor(m_renderer->get(), 255, 255, 255, 255);
				SDL_Rect rect = { x * cellSize + 1, y * cellSize + 1, cellSize - 2, cellSize - 2 };
				SDL_RenderFillRect(m_renderer->get(), &rect);
			} else if (pGrid->GetState(x, y) == GridState::Path) {
				SDL_SetRenderDrawColor(m_renderer->get(), 255, 0, 0, 255);
				SDL_Rect rect = { x * cellSize + 1, y * cellSize + 1, cellSize - 2, cellSize - 2 };
				SDL_RenderFillRect(m_renderer->get(), &rect);
			}
		}
	}

	const std::vector<Int2>& path = m_maze->GetPath();
	for (auto pos : path) {
		SDL_SetRenderDrawColor(m_renderer->get(), 255, 0, 0, 255);
		SDL_Rect rect = { pos.x * cellSize + 1, pos.y * cellSize + 1, cellSize - 2, cellSize - 2 };
		SDL_RenderFillRect(m_renderer->get(), &rect);
	}

	SDL_RenderPresent(m_renderer->get());
}
