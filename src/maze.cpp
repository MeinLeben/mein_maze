#include "pch.h"

#include "maze.h"

Maze::Maze(Int2 numTiles, Int2 sizePerTile) 
	: m_numTiles(numTiles)
	, m_sizePerTile(sizePerTile) {
	m_pathFinder = std::make_unique<PathFinder>();
	m_grid = std::make_unique<Grid>(m_numTiles.x, m_numTiles.y);
}

Maze::~Maze() {
	if (m_pathFinderThread.joinable()) {
		m_pathFinderThread.join();
	}
}

void Maze::Update() {
	if (m_onResult) {
		m_pathFinderCallback(m_result);
		m_onResult = false;
		m_isFinished = true;
	}
}

void Maze::GenerateRandomPattern(uint8_t weight) {
	if (!m_isFinished) {
		return;
	}

	for (int32_t y = 0; y < m_numTiles.y; y++) {
		for (int32_t x = 0; x < m_numTiles.x; x++) {
			m_grid->SetState(x, y, (GridState)(rand() % weight == 0));
		}
	}
}

void Maze::FindPath(const Int2& start, const Int2& destination, std::function<void(bool)> callback) {
	if (!m_isFinished) {
		return;
	}

	m_isFinished = false;
	m_pathFinderCallback = callback;
	if (m_pathFinderThread.joinable()) {
		m_pathFinderThread.join();
	}

	m_pathFinderThread = std::thread(&PathFinder::Execute, m_pathFinder.get(), start, destination, m_grid.get(), std::bind(&Maze::OnFindPathFinished, this, std::placeholders::_1));
}

void Maze::OnFindPathFinished(bool result) {
	m_onResult = true;
	m_result = result;
}