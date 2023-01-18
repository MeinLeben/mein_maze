#pragma once

#include "raycaster.h"
#include "view.h"

class Maze;

class MazeView : public IView, public IRayHandler {
public:
	MazeView(int32_t width, int32_t height, Maze* pMaze);
	MazeView(int32_t x, int32_t y, int32_t width, int32_t height, Maze* pMaze);

	~MazeView();

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

protected:
	virtual void HandleEvent(SDL_Event& event) override;

	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;

	virtual void OnHit(const Ray& ray) override;

private:
	void RenderRaycaster(SDL_Renderer* pRenderer, const Maze* pMaze);

	Maze* m_pMaze = nullptr;

	Int2 m_mousePosition = {};
	Int3 m_highlightColor = { 0, 255, 0 };

	bool m_showPath = false, m_showPlayer = false;

	Float2 m_playerPosition = {};
	float m_playerAngle = 0;
};
