#pragma once

#include "view.h"

class Maze;

class MazeView : public View {
public:
	MazeView(Maze* pMaze);

	inline void ShowPath(bool show) {
		m_showPath = show;
	}

protected:
	virtual void HandleEvent(SDL_Event& event) override;

	virtual void Update();
	virtual void Render();

private:
	Maze* m_pMaze = nullptr;

	Int2 m_mousePosition = {};
	Int3 m_highlightColor = { 0, 255, 0 };

	bool m_showPath = false;
};
