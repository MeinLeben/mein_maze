#pragma once

class Time {
public:
	static Time& Get() {
		return ms_instance;
	}

	inline void Update() {
		m_currentTimePoint = std::chrono::system_clock::now();
		std::chrono::duration<float> deltaTimePoint = m_currentTimePoint - m_previousTimePoint;
		m_previousTimePoint = m_currentTimePoint;
		m_deltaTime = deltaTimePoint.count();
	}

	inline float GetDeltaTime() const {
		return m_deltaTime;
	}

private:
	Time() = default;
	Time(const Time&) = delete;
	Time& operator=(const Time&) = delete;

	static Time ms_instance;

	std::chrono::system_clock::time_point m_currentTimePoint = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point m_previousTimePoint = std::chrono::system_clock::now();

	float m_deltaTime = 0.0f;
};
