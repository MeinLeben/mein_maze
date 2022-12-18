#include "pch.h"

#include "application.h"

#include "grid.h"

Application::Application(int32_t argc, char* argv[]) {
	std::srand(std::time(nullptr));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		throw std::runtime_error(SDL_GetError());
	}

	EventManager::Get().Register(this);

	m_maze = std::make_unique<Maze>();
	m_mazeView = std::make_unique<MazeView>(m_maze.get());
	m_mazeView->RegisterOnClose(std::bind(&Application::OnViewClose, this));

	m_gameView = std::make_unique<GameView>();
	m_gameView->RegisterOnClose(std::bind(&Application::OnViewClose, this));
}

Application::~Application() {
	EventManager::Get().DeRegister(this);

	SDL_Quit();
}

int32_t Application::Run() {
	while (m_isRunning) {
		EventManager::Get().Update();
		
		m_maze->Update();

		if (m_maze->IsSearching()) {
			m_mazeView->UpdateWindowTitle("Searching...");
		}

		if (!m_maze->IsSearching() && m_auto) {
			m_maze->GenerateRandomPattern(kMazeWeight);
			m_maze->FindPath(m_start, m_destination, std::bind(&Application::OnFindPathFinished, this, std::placeholders::_1));
		}

		ViewManager::Get().Update();
		ViewManager::Get().Render();
	}

	return m_exitCode;
}

void Application::HandleEvent(SDL_Event& event) {
	if (event.type == SDL_QUIT) {
		Shutdown();
		return;
	}

	if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_ESCAPE:
			Shutdown();
			return;
		case SDL_SCANCODE_C:
			m_maze->Clear();
			m_mazeView->ShowPath(false);
			break;
		case SDL_SCANCODE_SPACE:
			m_auto = false;
			if (!m_maze->IsSearching()) {
				m_mazeView->ShowPath(false);
				m_maze->GenerateRandomPattern(kMazeWeight);
			}
			break;
		case SDL_SCANCODE_RETURN: {
			m_auto = false;
			if (!m_maze->IsSearching()) {
				m_maze->FindPath(m_start, m_destination, std::bind(&Application::OnFindPathFinished, this, std::placeholders::_1));
			}
		} break;
		case SDL_SCANCODE_TAB:
			if (!m_maze->IsSearching()) {
				m_mazeView->ShowPath(false);
				m_auto = true;
				m_numberOfFails = 0;
			}
			break;
		default:
			break;
		}
	}
}

void Application::OnFindPathFinished(bool result) {
	m_mazeView->ShowPath(result);
	m_mazeView->UpdateWindowTitle("Path not found!");
	if (result) {
		m_mazeView->UpdateWindowTitle(m_auto ? "Path found in " + std::to_string(m_numberOfFails) + " retries!" : "Path found!");
		m_auto = false;
	} else {
		m_numberOfFails++;
	}
}

void Application::OnViewClose() {
	Shutdown();
}

