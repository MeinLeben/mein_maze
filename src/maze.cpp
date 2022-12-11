#include "pch.h"

#include "maze.h"

Maze::Maze() {
	m_pathFinder = std::make_unique<PathFinder>();
	m_grid = std::make_unique<Grid>(kGridSize, kGridSize);
}

void Maze::GenerateRandomPattern(uint8_t weight) {
	for (int32_t y = 0; y < kGridSize; y++) {
		for (int32_t x = 0; x < kGridSize; x++) {
			m_grid->SetState(x, y, (GridState)(rand() % weight == 0));
		}
	}
}

bool Maze::FindPath(const Int2& start, const Int2& destination) {
	std::promise<bool> found;
	std::future result = found.get_future();
	std::thread t(&PathFinder::Execute, m_pathFinder.get(), start, destination, m_grid.get(), std::move(found));
	t.join();
	return result.get();
}