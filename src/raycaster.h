#pragma once

class Maze;
class RayCaster;

struct RayCasterInfo {
	Float2 sourcePosition;
	float sourceAngle;
	Maze* pMaze;
};

struct Ray {
	Float2 startPosition;
	Float2 direction;
	float distanceTo;
};

class IRayHandler {
protected:
	virtual void OnHit(const Ray& ray) = 0;

private:
	RayCaster* m_pRayCaster = nullptr;
	friend class RayCaster;
};

class RayCaster {
public:
	static RayCaster& Get() {
		return ms_instance;
	}

	void RegisterHandler(IRayHandler* pHandler) {
		m_rayHandlers.insert(pHandler);
	}

	void DeRegisterHandler(IRayHandler* pHandler) {
		for (auto iter : m_rayHandlers) { 
			if (iter == pHandler) {
				m_rayHandlers.erase(iter);
			}
			return;
		}
	}

	void Execute(const RayCasterInfo& info);

private:
	RayCaster() = default;
	RayCaster(const RayCaster&) = delete;
	RayCaster& operator=(const RayCaster&) = delete;

	static RayCaster ms_instance;

	std::unordered_set<IRayHandler*> m_rayHandlers;
};