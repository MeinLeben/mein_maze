#pragma once

#include "sdl_wrapper.h"
#include "path_finder.h"

class MeinMaze {
public:
	MeinMaze();
	~MeinMaze();

	int32_t Run();

private:

	void Update();
	void Render();

	static void GenerateRandomPattern(const Int2& start, const Int2& destination, GridNode grid[][kGridSize], size_t size = kGridSize);

	std::unique_ptr<SDLWrapper::Window> m_window;
	std::unique_ptr<SDLWrapper::Renderer> m_renderer;

	GridNode m_grid[kGridSize][kGridSize] = {};

	Int2 m_start = { 0, 0 };
	Int2 m_destination = { kGridSize - 1, kGridSize - 1 };

	std::unique_ptr<PathFinder> m_pathFinder;

	bool m_auto = false;
	uint64_t m_numberOfFails = 0;
};
