#pragma once

#include <vector>
#include <stack>    // DFS Open List
#include <queue>    // BFS Open List
#include <list>     // Closed List
#include <set>      // Alternative Closed List
#include <iostream>

class Graph
{
public:
	// predefine classes
	struct Node;
	struct Edge
	{
		Node *connection;
		// default constructor
		Edge() : connection(NULL) { }
		// overloaded construct
		Edge(Node *node) : connection(node) { }
	};
	struct Node
	{
		int value;
		int dos;
		std::vector< Graph::Edge > connections;
		// default constructor
		Node() : value(0) {}
		// overloaded constructor
		Node(int val) : value(val) {}
	};
	~Graph()
	{
		// make sure to delete all nodes added to the graph
		for (auto iter = m_nodes.begin(); iter != m_nodes.end(); iter++)
			delete (*iter);
		m_nodes.clear();
	}
	Node *AddNode(int value)
	{
		m_nodes.push_back(new Node(value));
		return m_nodes.back();
	}
	void AddConnection(Node *n1, Node *n2/*, float weight */)
	{
		n1->connections.push_back(Edge(n2/*, weight*/));
		n2->connections.push_back(Edge(n1/*, weight*/));
	}
private:
	std::vector< Node * > m_nodes;
};

inline void PrintDFS(Graph::Node *startNode)
{
	std::stack<Graph::Node *> frontier;  // Open List
	std::set  <Graph::Node *> explored;  // Closed List

	frontier.push(startNode);
	explored.insert(startNode);
	while (!frontier.empty())
	{
		auto current = frontier.top();
		std::cout << (char)current->value << " ";
		frontier.pop();

		for each(Graph::Edge e in current->connections)
		{
			if (!explored.count(e.connection))
			{
				frontier.push(e.connection);
				explored.insert(e.connection);
			}
		}
	}
}


inline void PrintBFS(Graph::Node *startNode)
{
	std::list<Graph::Node *> frontier;  // Open List
	std::set  <Graph::Node *> explored;  // Closed List

	frontier.push_front(startNode);
	explored.insert(startNode);
	while (!frontier.empty())
	{
		auto current = frontier.front();
		std::cout << (char)current->value << " ";
		frontier.pop_back();

		for each(Graph::Edge e in current->connections)
		{
			if (!explored.count(e.connection))
			{
				frontier.push_front(e.connection);
				explored.insert(e.connection);
			}
		}
	}
}
typedef bool(*Compfunc)(const Graph::Node *, const Graph::Node *);

void CompareBFS(const Graph::Node *a, const Graph::Node *b)
{
	a->dos < b->dos;
}
void CompareDFS(const Graph::Node *a, const Graph::Node *b)
{
	a->dos > b->dos;
}

inline void PrintByComparison(Graph::Node *startNode,Compfunc comp)
{
	std::list<Graph::Node *> frontier;  // Open List
	std::set  <Graph::Node *> explored;  // Closed List

	frontier.push_back(startNode);
	explored.insert(startNode);
	while (!frontier.empty())
	{
		std::sort(frontier.begin(), frontier.end(), comp);

		auto current = frontier.front();
		std::cout << (char)current->value << " ";
		frontier.pop_back();

		for each(Graph::Edge e in current->connections)
		{
			if (!explored.count(e.connection))
			{
				e.connection->dos = current->dos + 1;
				frontier.push_front(e.connection);
				explored.insert(e.connection);
			}

			if (explored.count(e.connection) && e.connection->dos > current->dos + 1)
			e.connection->dos = current->dos + 1;
			
		}
	}
}
