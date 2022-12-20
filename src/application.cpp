#include "pch.h"

#include "application.h"

#include "grid.h"
#include "time.h"

Application::Application(int32_t argc, char* argv[]) {
	std::srand(std::time(nullptr));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		throw std::runtime_error(SDL_GetError());
	}

	EventManager::Get().Register(this);

	m_maze = std::make_unique<Maze>();
	m_mazeView = std::make_unique<MazeView>(m_maze.get());
	m_mazeView->RegisterOnClose(std::bind(&Application::OnViewClose, this));

	m_playerPosition.x = m_mazeView->GetTileSize() / 2.0f;
	m_playerPosition.y = m_mazeView->GetTileSize() / 2.0f;

	const int32_t x = m_mazeView->GetPosition().x + m_mazeView->GetSize().x + 10;
	const int32_t y = m_mazeView->GetPosition().y;

	m_gameView = std::make_unique<GameView>(x, y);
	m_gameView->RegisterOnClose(std::bind(&Application::OnViewClose, this));

	m_mazeView->Raise();
}

Application::~Application() {
	EventManager::Get().DeRegister(this);

	SDL_Quit();
}

int32_t Application::Run() {
	while (m_isRunning) {
		Time::Get().Update();

		EventManager::Get().Update();

		UpdateGame();
		UpdateMaze();

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

	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_LSHIFT:
			m_playerIsRunning = true;
			break;
		default:
			break;
		}
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
		case SDL_SCANCODE_LSHIFT:
			m_playerIsRunning = false;
			break;
		default:
			break;
		}
	}
}

void Application::UpdateGame() {
	float deltaTime = Time::Get().GetDeltaTime();
	int32_t numKeys = 0;
	const uint8_t* pKeyboardState = SDL_GetKeyboardState(&numKeys);

	const float speed = m_playerIsRunning ? kPlayerRunSpeed : kPlayerWalkSpeed;
	if (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_UP]) {
		m_playerPosition.x += cosf(m_playerAngle) * speed * deltaTime;
		m_playerPosition.y += sinf(m_playerAngle) * speed * deltaTime;
	}

	if (pKeyboardState[SDL_SCANCODE_S] || pKeyboardState[SDL_SCANCODE_DOWN]) {
		m_playerPosition.x -= cosf(m_playerAngle) * speed * deltaTime;
		m_playerPosition.y -= sinf(m_playerAngle) * speed * deltaTime;
	}

	if (pKeyboardState[SDL_SCANCODE_A] || pKeyboardState[SDL_SCANCODE_LEFT]) {
		m_playerAngle -= kPlayerRotationSpeed * deltaTime;
	}

	if (pKeyboardState[SDL_SCANCODE_D] || pKeyboardState[SDL_SCANCODE_RIGHT]) {
		m_playerAngle += kPlayerRotationSpeed * deltaTime;
	}

	m_mazeView->SetPlayerPosition(m_playerPosition);
	m_mazeView->SetPlayerAngle(m_playerAngle);
	m_mazeView->ShowPlayer(true);
}

void Application::UpdateMaze() {
	m_maze->Update();

	if (m_maze->IsSearching()) {
		m_mazeView->UpdateWindowTitle("Searching...");
	}

	if (!m_maze->IsSearching() && m_auto) {
		m_maze->GenerateRandomPattern(kMazeWeight);
		m_maze->FindPath(m_start, m_destination, std::bind(&Application::OnFindPathFinished, this, std::placeholders::_1));
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

