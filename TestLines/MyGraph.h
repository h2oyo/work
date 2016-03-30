#pragma once
#pragma once
#include <list>
#include <cassert>
#include <cmath>
#include <map>
#include <cstring>
#include <vector>

// some vector stuff to help out 
struct Vector2
{
	float x, y;
	Vector2() {}
	Vector2(float x, float y) :x(x), y(y) {}
};

inline float distance(Vector2 a, Vector2 b)
{
	return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

/*
Right now, addNode, connectNodes, and draw are really all
that matter. Try to make sure you understand them.

Don't copy this code, just look at the functions.
*/
class MyGraph
{
public:
	// just storing position data
	struct Node { Vector2 data; };

	// connected nodes and weights
	struct Edge { Node *a, *b; float weight; };


	MyGraph() : isBaked(true) { }
	virtual ~MyGraph() { }

	Node  *AddNode(const Vector2 &data)
	{
		isBaked = false;
		nodes.push_back(Node{ data });
		return &nodes.back();
	}

	//doesn't ensure the node is already in our graph >__>, don't like this,
	//but won't fight the tutorial for now.
	void ConnectNodes(Node *nodeA, Node *nodeB, float weight)
	{
		assert(weight > 0);
		isBaked = false;
		edges.push_front(Edge{ nodeA, nodeB, weight });
	}


	// Loop through all nodes/edges and remove all trace of the node provided
	bool  RemoveNode(Node *pNode)
	{
		// look through all edges to see if the start or end node
		// contains either of the target node
		for (auto it = edges.begin(); it != edges.end(); it++)
			if (it->a == pNode || it->b == pNode)
				it = edges.erase(it); // erase returns an iterator to the next element

									  // go through each node and if a node's address is the target node,
									  // delete it and get out of here.
		for (auto it = nodes.begin(); it != nodes.end(); it++)
			if (&(*it) == pNode)
			{
				nodes.erase(it);
				isBaked = false;
				return true;
			}
		return false;
	}

	// finding a node is based on the data.
	// since floating point error is a thing,
	// find node will the node that is closest to the param position.
	Node  *FindNode(Vector2 data)
	{
		float md = INFINITY;
		Node *r = nullptr;
		for (auto it = nodes.begin(); it != nodes.end(); it++)  // loop through each node
		{                                                       // using an iterator here
			float d = distance(it->data, data);  // evaluate distance
			if (d < md)
			{
				md = d;     // if distance is closer,
				r = &(*it); // set the retval to the address of our node.
			}
		}
		return r;
	}


	// calculate the adjacency matrix and put it into a contiguous data store
	// makes future problems simpler in several ways.
	void bakeData()
	{
		if (isBaked) return;

		// clear current baked data
		adjacencyMatrix.clear();
		bakedPositions.clear();

		bakedPositions.resize(size());
		adjacencyMatrix.resize(size()*size(), 0);

		// Since nodes don't have indices until they are baked,
		// and edges will need to know indices for the adjacency matrix,
		// we can make a map that stores the address of the actual node and
		// the index of that node's data in the position vector.
		std::map<Node*, unsigned> bakeMap;

		unsigned i = 0;
		for (auto it = nodes.begin(); it != nodes.end(); it++)
		{
			// use address as key and index as value
			bakeMap[&(*it)] = i;
			bakedPositions[i++] = it->data;
		}

		for each(auto e in edges)
		{
			// so that we can access the index using memory address!
			// (Because edge only stores pointers, but we need indices for adj matrix!)
			adjacencyMatrix[bakeMap[e.a] * size() + bakeMap[e.b]] = e.weight;
		}
		isBaked = true;
	}


	// fetch relevant data for baked data.
	unsigned size() { return nodes.size(); }
	Vector2 *getPositions() { assert(isBaked); return bakedPositions.data(); }
	float   *getAdjacencyMatrix() { assert(isBaked); return adjacencyMatrix.data(); }


	void draw();

protected:
	bool isBaked;   // Whether or not adjacency data has been baked into a matrix

	std::list<Node> nodes;  // list of nodes
	std::list<Edge> edges;  // list of edges

	std::vector<Vector2> bakedPositions;  // positions in contiguous data store (baked)
	std::vector<float>   adjacencyMatrix; // adjacency matrix (baked)




public:
	static MyGraph makeGrid(Vector2 min, Vector2 max, unsigned rows, unsigned cols)
	{
		MyGraph r;
		float xOffset = (max.x - min.x) / float(rows - 1);
		float yOffset = (max.y - min.y) / float(rows - 1);

		for (unsigned i = 0; i < rows; ++i)
			for (unsigned j = 0; j < cols; ++j)
				r.AddNode(Vector2(xOffset*i + min.x, yOffset*j + min.y));

		for (auto it = r.nodes.begin(); it != r.nodes.end(); ++it)
		{
			auto a = r.FindNode({ it->data.x + xOffset, it->data.y });
			if (a != &(*it)) r.ConnectNodes(&(*it), a, distance(it->data, a->data));

			auto b = r.FindNode({ it->data.x - xOffset, it->data.y });
			if (b != &(*it)) r.ConnectNodes(&(*it), b, distance(it->data, b->data));

			auto c = r.FindNode({ it->data.x, it->data.y + yOffset });
			if (c != &(*it)) r.ConnectNodes(&(*it), c, distance(it->data, c->data));

			auto d = r.FindNode({ it->data.x, it->data.y - yOffset });
			if (d != &(*it)) r.ConnectNodes(&(*it), d, distance(it->data, d->data));



			a = r.FindNode({ it->data.x + xOffset, it->data.y + yOffset });
			if (a != &(*it)) r.ConnectNodes(&(*it), a, distance(it->data, a->data));

			b = r.FindNode({ it->data.x + xOffset, it->data.y - yOffset });
			if (b != &(*it)) r.ConnectNodes(&(*it), b, distance(it->data, b->data));

			c = r.FindNode({ it->data.x - xOffset, it->data.y + yOffset });
			if (c != &(*it)) r.ConnectNodes(&(*it), c, distance(it->data, c->data));

			d = r.FindNode({ it->data.x - xOffset, it->data.y - yOffset });
			if (d != &(*it)) r.ConnectNodes(&(*it), d, distance(it->data, d->data));
		}

		r.bakeData();
		return r;
	}
};