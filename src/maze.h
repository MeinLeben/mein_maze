#pragma once

#include "path_finder.h"
#include "grid.h"

class Maze {
public:
	Maze();

	void GenerateRandomPattern(uint8_t weight);
	bool FindPath(const Int2& start, const Int2& destination);

	const std::vector<Int2> GetPath() const {
		return m_pathFinder->GetLastFoundPath();
	}

	const Grid* GetGrid() const {
		return m_grid.get();
	}

private:
	std::unique_ptr<PathFinder> m_pathFinder;
	std::unique_ptr<Grid> m_grid;
	std::unique_ptr<Grid> m_grids[2];
};