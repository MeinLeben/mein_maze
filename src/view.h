#pragma once

#include "event.h"
#include "sdl_wrapper.h"

class IView : public IEventHandler {
public:
	IView(const std::string& title, int32_t width, int32_t height);
	IView(const std::string& title, int32_t x, int32_t y, int32_t width, int32_t height);

	virtual ~IView();

	inline void Show(bool show) {
		m_isVisible = show;
		if (m_isVisible) {
			SDL_ShowWindow(m_window->get());
		} else {
			SDL_HideWindow(m_window->get());
		}
	}

	inline bool IsVisible() const {
		return m_isVisible;
	}

	inline void Raise() {
		SDL_RaiseWindow(m_window->get());
	}

	inline uint32_t GetViewId() const {
		return SDL_GetWindowID(m_window->get());
	}

	inline void UpdateWindowTitle(const std::string& title) {
		SDL_SetWindowTitle(m_window->get(), title.c_str());
	}

	void RegisterOnClose(std::function<void()> callback) {
		m_onCloseCallback = callback;
	}

	Int2 GetPosition() const {
		Int2 position;
		SDL_GetWindowPosition(m_window->get(), &position.x, &position.y);
		return position;
	}

	Int2 GetSize() const {
		Int2 size;
		SDL_GetWindowSize(m_window->get(), &size.x, &size.y);
		return size;
	}

protected:
	virtual void HandleEvent(SDL_Event& event);

	virtual void Update() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;

	inline SDL_Window* GetWindow() const {
		return m_window->get();
	}

	inline SDL_Renderer* GetRenderer() const {
		return m_renderer->get();
	}

private:
	std::unique_ptr<SDLWrapper::Window> m_window;
	std::unique_ptr<SDLWrapper::Renderer> m_renderer;

	std::function<void()> m_onCloseCallback;

	bool m_isVisible = true;

	friend class ViewManager;
};

class ViewManager {
public:
	static ViewManager& Get() {
		return ms_instance;
	}

	inline void Register(IView* pView) {
		m_views.insert(pView);
	}

	inline void DeRegister(IView* pView) {
		m_views.erase(pView);
	}

	void Update();

	void PreRender();
	void Render();

private:
	ViewManager() = default;
	ViewManager(const ViewManager&) = delete;
	ViewManager& operator=(const ViewManager&) = delete;

	static ViewManager ms_instance;
	std::unordered_set<IView*> m_views;
};
