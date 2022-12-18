#include "pch.h"

#include "view.h"

ViewManager ViewManager::ms_instance;

View::View(const std::string& title, int32_t x, int32_t y, int32_t width, int32_t height) {
	ViewManager::Get().Register(this);
	EventManager::Get().Register(this);

	m_window = std::make_unique<SDLWrapper::Window>(title.c_str(), x, y, width, height, 0);
	m_renderer = std::make_unique<SDLWrapper::Renderer>(m_window->get(), -1, SDL_RENDERER_ACCELERATED);
}

View::View(const std::string& title, int32_t width, int32_t height) 
	: View(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height) {
}

View::~View() {
	EventManager::Get().DeRegister(this);
	ViewManager::Get().DeRegister(this);
}

void View::HandleEvent(SDL_Event& event) {
	if (event.window.windowID != SDL_GetWindowID(GetWindow())) {
		return;
	}

	if (event.type == SDL_WINDOWEVENT) {
		switch (event.window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			if (m_onCloseCallback) {
				m_onCloseCallback();
			}
			return;
		default:
			break;
		}
	}
}

void ViewManager::Update() {
	for (auto view : m_views) {
		view->Update();
	}
}

void ViewManager::Render() {
	for (auto view : m_views) {
		view->Render();
	}
}
