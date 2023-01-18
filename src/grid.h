#pragma once

class Grid {
public:
	Grid(int32_t width, int32_t height) 
		: m_width(width)
		, m_height(height) {
		m_pContainer = new GridState[width * height];
		Clear();
	}

	~Grid() {
		delete[] m_pContainer;
	}

	inline void Clear() {
		memset(m_pContainer, 0, sizeof(GridState) * m_width * m_height);
	}

	inline void SetState(uint32_t x, uint32_t y, const GridState& state) {
		assert(y * m_width + x < m_width * m_height);
		m_pContainer[y * m_width + x] = state;
	}

	inline const GridState& GetState(uint32_t x, uint32_t y) const {
		assert(y * m_width + x < m_width * m_height);
		return m_pContainer[y * m_width + x];
	}

	inline const GridState& GetState(Int2 position) const {
		return GetState(position.x, position.y);
	}

	inline int32_t GetWidth() const {
		return m_width;
	}

	inline int32_t GetHeight() const {
		return m_height;
	}

private:
	GridState* m_pContainer = nullptr;
	int32_t m_width, m_height;
};