#include "pch.h"

#include "game_view.h"

#include "time.h"

const static std::string kTitle = "Mein Game";

const static int32_t kWindowWidth = 720;
const static int32_t kWindowHeight = 405;

GameView::GameView(int32_t x, int32_t y) 
	: View(kTitle, x, y, kWindowWidth, kWindowHeight) {
}

void GameView::HandleEvent(SDL_Event& event) {
	View::HandleEvent(event);
}

void GameView::Update() {
	float deltaTime = Time::Get().GetDeltaTime();
	int32_t numKeys = 0;
	const uint8_t* pKeyboardState = SDL_GetKeyboardState(&numKeys);

	if (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_UP]) {
		m_playerPosition.x += sinf(m_playerAngle) * 5.0f * deltaTime;
		m_playerPosition.y += cosf(m_playerAngle) * 5.0f * deltaTime;
	}

	if (pKeyboardState[SDL_SCANCODE_S] || pKeyboardState[SDL_SCANCODE_DOWN]) {
		m_playerPosition.x -= sinf(m_playerAngle) * 5.0f * deltaTime;
		m_playerPosition.y -= cosf(m_playerAngle) * 5.0f * deltaTime;
	}

	if (pKeyboardState[SDL_SCANCODE_A] || pKeyboardState[SDL_SCANCODE_LEFT]) {
		m_playerAngle -= 0.8f * deltaTime;
	}

	if (pKeyboardState[SDL_SCANCODE_D] || pKeyboardState[SDL_SCANCODE_RIGHT]) {
		m_playerAngle += 0.8f * deltaTime;
	}

}

void GameView::Render() {
	SDL_SetRenderDrawColor(GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(GetRenderer());

	SDL_RenderPresent(GetRenderer());
}
