#pragma once

#include "view.h"

class Maze;

class MazeView : public View {
public:
	MazeView(int32_t x, int32_t y, Maze* pMaze);
	MazeView(Maze* pMaze);

	inline void ShowPath(bool show) {
		m_showPath = show;
	}

	inline void ShowPlayer(bool show) {
		m_showPlayer = show;
	}

	inline void SetPlayerPosition(Float2 position) {
		m_playerPosition = position;
	}

	inline void SetPlayerAngle(float angle) {
		m_playerAngle = angle;
	}

	inline int32_t GetCellSize() const {
		return kWindowHeight / kGridSize;
	}

protected:
	virtual void HandleEvent(SDL_Event& event) override;

	virtual void Update() override;
	virtual void Render() override;

private:
	const static int32_t kWindowWidth = 640;
	const static int32_t kWindowHeight = 640;

	Maze* m_pMaze = nullptr;

	Int2 m_mousePosition = {};
	Int3 m_highlightColor = { 0, 255, 0 };

	bool m_showPath = false, m_showPlayer = false;

	Float2 m_playerPosition = {};
	float m_playerAngle = 0;
};
