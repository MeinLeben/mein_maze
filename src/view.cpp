#include "pch.h"

#include "view.h"

ViewManager ViewManager::ms_instance;

IView::IView(const std::string& title, int32_t x, int32_t y, int32_t width, int32_t height) {
	ViewManager::Get().Register(this);
	EventManager::Get().Register(this);

	m_window = std::make_unique<SDLWrapper::Window>(title.c_str(), x, y, width, height, 0);
	m_renderer = std::make_unique<SDLWrapper::Renderer>(m_window->get(), -1, SDL_RENDERER_ACCELERATED);
}

IView::IView(const std::string& title, int32_t width, int32_t height) 
	: IView(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height) {
}

IView::~IView() {
	EventManager::Get().DeRegister(this);
	ViewManager::Get().DeRegister(this);
}

void IView::HandleEvent(SDL_Event& event) {
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

void ViewManager::PreRender() {
	for (auto view : m_views) {
		view->PreRender();
	}
}

void ViewManager::Render() {
	for (auto view : m_views) {
		view->Render();
	}
}
