#pragma once

class EventHandler {
protected:
	virtual void HandleEvent(SDL_Event& event) = 0;

private:
	friend class EventManager;
};

class EventManager {
public:
	static EventManager& Get() {
		return ms_instance;
	}

	inline void Register(EventHandler* pEventHandler) {
		m_eventHandlers.insert(pEventHandler);
	}

	inline void DeRegister(EventHandler* pEventHandler) {
		m_eventHandlers.erase(pEventHandler);
	}

	void Update();
private:
	EventManager() = default;
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;

	static EventManager ms_instance;
	std::unordered_set<EventHandler*> m_eventHandlers;
};
