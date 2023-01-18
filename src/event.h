#pragma once

class IEventHandler {
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

	inline void Register(IEventHandler* pEventHandler) {
		m_eventHandlers.insert(pEventHandler);
	}

	inline void DeRegister(IEventHandler* pEventHandler) {
		m_eventHandlers.erase(pEventHandler);
	}

	void Update();
private:
	EventManager() = default;
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;

	static EventManager ms_instance;
	std::unordered_set<IEventHandler*> m_eventHandlers;
};
