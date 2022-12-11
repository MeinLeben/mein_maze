#pragma once

class Grid {
public:
	Grid(int32_t width, int32_t height) 
		: m_width(width)
		, m_height(height) {
		m_pContainer = new GridState[width * height];
	}

	~Grid() {
		delete[] m_pContainer;
	}

	void SetState(uint32_t x, uint32_t y, const GridState& state) {
		assert(y * m_width + x < m_width * m_height);
		m_pContainer[y * m_width + x] = state;
	}

	const GridState& GetState(uint32_t x, uint32_t y) const {
		assert(y * m_width + x < m_width * m_height);
		return m_pContainer[y * m_width + x];
	}

private:
	GridState* m_pContainer = nullptr;
	int32_t m_width, m_height;
};