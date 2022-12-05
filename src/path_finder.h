#pragma once

class PathFinder {
public:
	~PathFinder() {
		Reset();
	}

	bool Execute(const Int2& start, const Int2& destination, GridNode grid[][kGridSize], size_t size = kGridSize);

private:
	struct Node {
		Node* m_pParent = nullptr;
		Int2 pos = {};
		float g = 0.0f, h = 0.0f, f = 0.0f;
	};

	std::unordered_set<Node*> m_openList;
	std::deque<Node*> m_closedList;

	void Reset();

	template <typename T>
	Node* FindNode(const T& list, const Int2& position) const;

	void TracePath(std::deque<Node*>& closedList, GridNode grid[][kGridSize], size_t size = kGridSize);

	float CalculateManhattanHeuristic(const Int2& current, const Int2& destination);
	float CalculateEuclideanHeuristic(const Int2& current, const Int2& destination);
};
