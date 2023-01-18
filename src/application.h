#pragma once

#include "event.h"
#include "game_view.h"
#include "maze.h"
#include "maze_view.h"

class Application : public IEventHandler {
public:
	Application(int32_t argc, char* argv[]);
	~Application();

	int32_t Run();

protected:
	virtual void HandleEvent(SDL_Event& event);

private:
	void UpdateGame();
	void UpdateMaze();

	void OnFindPathFinished(bool result);
	void OnViewClose();

	inline void Shutdown(int32_t exitCode = 0) {
		m_exitCode = exitCode;
		m_isRunning = false;
	}

	int32_t m_exitCode = 0;
	bool m_isRunning = true;

	std::unique_ptr<Maze> m_maze;

	std::unique_ptr<MazeView> m_mazeView;
	std::unique_ptr<GameView> m_gameView;


	Int2 m_start = {};
	Int2 m_destination = {};
	uint64_t m_numberOfFails = 0;
	bool m_auto = false;

	Float2 m_playerPosition = {};
	float m_playerAngle = 0;
	bool m_playerIsRunning = false;
};
