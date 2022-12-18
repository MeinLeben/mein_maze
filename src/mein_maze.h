#pragma once

#include "event.h"
#include "maze.h"
#include "maze_2d.h"
#include "sdl_wrapper.h"

class MeinMaze : public EventHandler {
public:
	MeinMaze();
	~MeinMaze();

	int32_t Run();

protected:
	virtual void HandleEvent(SDL_Event& event);

private:
	void Update();
	void Render();

	void OnFindPathFinished(bool result);

	inline void UpdateWindowTitle(const std::string& title) {
		SDL_SetWindowTitle(m_window->get(), title.c_str());
	}

	inline void Shutdown(int32_t exitCode = 0) {
		m_exitCode = exitCode;
		m_isRunning = false;
	}

	std::unique_ptr<Maze2d> m_maze2d;
	std::unique_ptr<Maze> m_maze;

	std::unique_ptr<SDLWrapper::Window> m_window;
	std::unique_ptr<SDLWrapper::Renderer> m_renderer;

	Int2 m_start = { 0, 0 };
	Int2 m_destination = { kGridSize - 1, kGridSize - 1 };

	bool m_isPathFound = false;
	uint64_t m_numberOfFails = 0;

	int32_t m_exitCode = 0;
	bool m_isRunning = true;

	bool m_auto = false;

	Int2 m_mousePosition = {};
	Int3 m_highlightColor = {0, 255, 0};
};
