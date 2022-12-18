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

protected:
	virtual void HandleEvent(SDL_Event& event) override;

	virtual void Update() override;
	virtual void Render() override;

private:
	Maze* m_pMaze = nullptr;

	Int2 m_mousePosition = {};
	Int3 m_highlightColor = { 0, 255, 0 };

	bool m_showPath = false;
};
