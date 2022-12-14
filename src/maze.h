#pragma once

#include "path_finder.h"
#include "grid.h"

class Maze {
public:
	Maze();
	~Maze();

	void Update();

	void GenerateRandomPattern(uint8_t weight);
	void FindPath(const Int2& start, const Int2& destination, std::function<void(bool)> callback);

	inline void Clear() {
		if (m_isFinished) {
			m_grid->Clear();
		}
	}

	inline bool IsSearching() const {
		return !m_isFinished;
	}

	inline const std::vector<Int2> GetPath() const {
		return m_pathFinder->GetLastFoundPath();
	}

	inline const Grid* GetGrid() const {
		return m_grid.get();
	}

private:
	void OnFindPathFinished(bool result);

	std::thread m_pathFinderThread;

	std::unique_ptr<PathFinder> m_pathFinder;
	std::unique_ptr<Grid> m_grid;
	std::function<void(bool)> m_pathFinderCallback;

	bool m_isFinished = true;
	std::atomic<bool> m_onResult = false;
	std::atomic<bool> m_result = false;
};