#pragma once

#include "sdl_wrapper.h"

#include "maze.h"

class MeinMaze {
public:
	MeinMaze();
	~MeinMaze();

	int32_t Run();

private:
	void Update();
	void Render();

	std::unique_ptr<Maze> m_maze;

	std::unique_ptr<SDLWrapper::Window> m_window;
	std::unique_ptr<SDLWrapper::Renderer> m_renderer;

	Int2 m_start = { 0, 0 };
	Int2 m_destination = { kGridSize - 1, kGridSize - 1 };

	bool m_auto = false;
	uint64_t m_numberOfFails = 0;
};
