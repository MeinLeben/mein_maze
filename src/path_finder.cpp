#include "pch.h"

#include "path_finder.h"

#include "grid.h"

PathFinder::~PathFinder() {
	Reset();
}

void PathFinder::Execute(const Int2& start, const Int2& destination, const Grid* pGrid, std::function<void(bool)> callback) {
	bool result = false;
	m_path.clear();

	if (pGrid->GetState(start.x, start.y) == GridState::Collision ||
		pGrid->GetState(destination.x, destination.y) == GridState::Collision) {
		callback(result);
		return;
	}

	m_openList.insert(new Node{ nullptr, start, 0.0f, 0.0f, 0.0f });

	while (!m_openList.empty()) {
		Node* q = nullptr;
		for (auto iter : m_openList) {
			if (q == nullptr) {
				q = iter;
				continue;
			}

			if (iter->f < q->f) {
				q = iter;
			}
		}

		m_openList.erase(q);

		for (int32_t y = -1; y <= 1; y++) {
			for (int32_t x = -1; x <= 1; x++) {
				if (x == 0 && y == 0)
					continue;

				const Int2 childPos = { q->pos.x + x, q->pos.y + y };
				if (childPos.x < 0 || childPos.y < 0 ||
					childPos.x >= kGridSize || childPos.y >= kGridSize) {
					continue;
				}

				if (childPos.x == destination.x && childPos.y == destination.y) {
					m_closedList.push_back(q);
					m_closedList.push_back(new Node{ q, childPos, 0.0f, 0.0f, 0.0f });
					result = true;
					break;
				}

				if (pGrid->GetState(childPos.x, childPos.y) == GridState::Collision)
					continue;
/*
				if (abs(x) == 1 && abs(y) == 1) {
					const int32_t nx = q->pos.x + x;
					const int32_t ny = q->pos.y + y;

					if (nx >= 0 && nx < kGridSize && grid[nx][q->pos.y] &&
						ny >= 0 && ny < kGridSize&& grid[q->pos.x][ny]) {
							continue;
					}
				}
*/
				float g = q->g + 1.0f;
				if (abs(x) == 1 && abs(y) == 1) {
					g += 0.414;
				}

//				const float h = CalculateManhattanHeuristic(childPos, destination);
				const float h = CalculateEuclideanHeuristic(childPos, destination);
				const float f = g + h;

				const Node* pClosedDuplicate = FindNode(m_closedList, childPos);
				if (pClosedDuplicate && pClosedDuplicate->f < f)
					continue;

				Node* pOpenDuplicate = FindNode(m_openList, childPos);
				if (pOpenDuplicate && pOpenDuplicate->f < f) {
					continue;
				} else {
					m_openList.erase(pOpenDuplicate);
				}

				m_openList.insert(new Node{ q, childPos, g, h, f });
			}
		}

		if (result) {
			break;
		}

		m_closedList.push_back(q);
	}

	TracePath(m_closedList);

	Reset();

	callback(result);
}

void PathFinder::Reset() {
	for (auto iter : m_openList) {
		delete iter;
	}

	m_openList.clear();

	for (auto iter : m_closedList) {
		delete iter;
	}

	m_closedList.clear();
}

template <typename T>
PathFinder::Node* PathFinder::FindNode(const T& list, const Int2& position) const {
	for (auto iter : list) {
		if (iter->pos.x == position.x && iter->pos.y == position.y) {
			return iter;
		}
	}
	return nullptr;
}

void PathFinder::TracePath(std::deque<Node*>& closedList) {
	Node* pParent = nullptr;
	while (!closedList.empty()) {
		Node* pNode = closedList.back();
		if (!pParent || pNode == pParent) {
			m_path.push_back(pNode->pos);
			pParent = pNode->m_pParent;
		}
		delete pNode;
		closedList.pop_back();
	}
}

float PathFinder::CalculateManhattanHeuristic(const Int2& current, const Int2& destination) {
	return fabs(current.x - destination.x) + abs(current.y - destination.y);
}

float PathFinder::CalculateEuclideanHeuristic(const Int2& current, const Int2& destination) {
	return sqrt(((current.x - destination.x) * (current.x - destination.x)) + (current.y - destination.y) * (current.y - destination.y));
}
