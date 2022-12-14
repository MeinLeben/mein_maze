#pragma once

class Grid;

class PathFinder {
public:
	~PathFinder();

	void Execute(const Int2& start, const Int2& destination, const Grid* pGrid, std::function<void(bool)> callback);

	const std::vector<Int2>& GetLastFoundPath() const {
		return m_path;
	}

private:
	struct Node {
		Node* m_pParent = nullptr;
		Int2 pos = {};
		float g = 0.0f, h = 0.0f, f = 0.0f;
	};

	std::thread m_thread = {};

	std::unordered_set<Node*> m_openList;
	std::deque<Node*> m_closedList; //CFR TODO: Do we really need a deque here?
	std::vector<Int2> m_path;

	void Reset();

	template <typename T>
	Node* FindNode(const T& list, const Int2& position) const;

	//CFR TODO: Do we really need this method still here?
	void TracePath(std::deque<Node*>& closedList);

	float CalculateManhattanHeuristic(const Int2& current, const Int2& destination);
	float CalculateEuclideanHeuristic(const Int2& current, const Int2& destination);
};
