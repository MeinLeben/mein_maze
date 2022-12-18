#include "pch.h"

#include "event.h"

EventManager EventManager::ms_instance;

void EventManager::Update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		for (auto eventHandler : m_eventHandlers) {
			eventHandler->HandleEvent(event);
		}
	}
}
