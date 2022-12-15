#include "pch.h"

#include "mein_maze.h"
#include "grid.h"

static const std::string kTitle = "MeinMaze";

MeinMaze::MeinMaze() {
	std::srand(std::time(nullptr));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		throw std::runtime_error(SDL_GetError());
	}

	m_maze = std::make_unique<Maze>();

	m_window = std::make_unique<SDLWrapper::Window>(kTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kWindowWidth, kWindowHeight, 0);
	m_renderer = std::make_unique<SDLWrapper::Renderer>(m_window->get(), -1, SDL_RENDERER_ACCELERATED);
}

MeinMaze::~MeinMaze() {
	SDL_Quit();
}

int32_t MeinMaze::Run() {
	while (m_isRunning) {
		Update();
		Render();
	}

	return m_exitCode;
}

void MeinMaze::Update() {
	uint32_t mouseState = SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
	UpdateWindowTitle(std::to_string(m_mousePosition.x) + std::string(" ") + std::to_string(m_mousePosition.y));

	SDL_Event event = {};
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			Shutdown();
			return;
		}

		if (event.type == SDL_WINDOWEVENT) {
			switch (event.window.type) {
			case SDL_WINDOWEVENT_CLOSE:
				Shutdown();
				return;
			default:
				break;
			}
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

		if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
				Shutdown();
				return;
			case SDL_SCANCODE_C:
				m_maze->Clear();
				m_isPathFound = false;
				break;
			case SDL_SCANCODE_SPACE:
				m_auto = false;
				UpdateWindowTitle(kTitle);
				if (!m_maze->IsSearching()) {
					m_isPathFound = false;
					m_maze->GenerateRandomPattern(kMazeWeight);
				}
				break;
			case SDL_SCANCODE_RETURN: {
				m_auto = false;
				if (!m_maze->IsSearching()) {
					m_maze->FindPath(m_start, m_destination, std::bind(&MeinMaze::OnFindPathFinished, this, std::placeholders::_1));
				}
			} break;
			case SDL_SCANCODE_TAB:
				if (!m_maze->IsSearching()) {
					m_auto = true;
					m_numberOfFails = 0;
					m_isPathFound = false;
				}
				break;
			default:
				break;
			}
		}
	}

	m_maze->Update();

	if (m_maze->IsSearching()) {
		UpdateWindowTitle("Searching...");
	} else if (m_auto) {
		m_maze->GenerateRandomPattern(kMazeWeight);
		m_maze->FindPath(m_start, m_destination, std::bind(&MeinMaze::OnFindPathFinished, this, std::placeholders::_1));
	}
}

void MeinMaze::OnFindPathFinished(bool result) {
	m_isPathFound = result;

	UpdateWindowTitle("Path not found!");
	if (result) {
		UpdateWindowTitle(m_auto ? "Path found in " + std::to_string(m_numberOfFails) + " retries!" : "Path found!");
		m_auto = false;
	} else {
		m_numberOfFails++;
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
				SDL_Rect rect = { x * cellSize + 1, y * cellSize + 1, cellSize - 1, cellSize - 1 };
				SDL_RenderFillRect(m_renderer->get(), &rect);
			}
		}
	}

	if (m_isPathFound) {
		const std::vector<Int2>& path = m_maze->GetPath();
		for (auto pos : path) {
			SDL_SetRenderDrawColor(m_renderer->get(), 255, 0, 0, 255);
			SDL_Rect rect = { pos.x * cellSize + 1, pos.y * cellSize + 1, cellSize - 1, cellSize - 1 };
			SDL_RenderFillRect(m_renderer->get(), &rect);
		}
	}

	const int32_t x = ((m_mousePosition.x - 2) / cellSize) * cellSize;
	const int32_t y = ((m_mousePosition.y - 3) / cellSize) * cellSize;
	SDL_SetRenderDrawColor(m_renderer->get(), m_highlightColor.x, m_highlightColor.y, m_highlightColor.z, 255);
	const SDL_Rect rect = {x + 1, y + 1, cellSize - 1, cellSize - 1};
	SDL_RenderFillRect(m_renderer->get(), &rect);
	SDL_RenderPresent(m_renderer->get());
}
