//Inspired from A Faster Algorithm for Betweenness Centrality paper.

//Takes O(nm) and O(nm + n^2log n) time on unweighted and weighted networks.

#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <list>

#include <cstring>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

// The new adjacency list type.
typedef vector<vector<int>> adjacency_list;

// BFS algorithm is used to calculate all the single source shortest paths in a non weighted graph and the source's closeness.
void bfs_SSSP(int src, int n, stack<int> &visitStack, vector<int> &sigma, list<int> *pred, adjacency_list &adjList)
{

	// Vector that holds the distances from the source.
	vector<int> dist;
	dist.resize(n, -1);
	dist[src] = 0;

	// Queue used for the Bfs algorithm.
	queue<int> visitQueue;
	visitQueue.push(src);

	// While there are still elements in the queue.
	while (!visitQueue.empty())
	{
		// Pop the first.
		int v = visitQueue.front();
		visitQueue.pop();
		visitStack.push(v);

		// Check the neighbors w of v.
		for (vector<int>::iterator it = adjList[v].begin(); it != adjList[v].end(); it++)
		{
			int w = *it;
			// Node w found for the first time?
			if (dist[w] < 0)
			{
				visitQueue.push(w);
				dist[w] = dist[v] + 1;
			}
			// Is the shortest path to w via u?
			if (dist[w] == dist[v] + 1)
			{
				pred[w].push_back(v);
				sigma[w] += sigma[v];
			}
		}
	}
}

// Reads an input file and fills up the adjacency list as well as the edges.
void readGraph(int &n, adjacency_list &adjList)
{
	cout << "read start" << endl;
	int e = 0; // Total number of edges (for statistics).
	ifstream fp;
	cout << "opening" << endl;
	fp.open("inn.txt");

	fp >> n;
	adjList.resize(n, vector<int>());
	cout << n << endl;
	// Read the nodes and the edges, one by one, and fill up adjList.
	int start, end;
	while (!fp.eof())
	{
		e += 1;
		fp >> start;
		fp >> end;
		cout << start << "-" << end << "\n";

		adjList[start].push_back(end);
		adjList[end].push_back(start);//reverse links cluster head to end
	}
	//adjList.resize(n);

	// Print statistics after reading.
	cout << "\n==================="
		 << "\nINPUT GRAPH STATS"
		 << "\n>#ofNodes: " << n
		 << "\n>#ofEdges: " << e
		 << "\n===================\n\n";
	fp.close();
}

// Clears the variables or re-initializes to 0, so that they are ready for the next loop.
void resetVariables(int src, int n, list<int> *pred, vector<int> &sigma, vector<float> &delta)
{
	for (int i = 0; i < n; i++)
	{
		pred[i].clear();
	}

	sigma.clear();
	sigma.resize(n, 0);
	sigma[src] = 1;

	delta.clear();
	delta.resize(n, 0);
}

// Prints Node Betweenness Centrality.
void printNodeBetweenness(int n, vector<float> nodeBetweenness, bool normalize)
{
	float nrml = 1;
	if (normalize)
	{
		nrml = (n - 1) * (n - 2);
	}
	ofstream out;
	out.open("out_node_betweenness.txt");
	cout << endl
		 << "> Node Betweenness Centrality" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "Node " << i << ": " << nodeBetweenness[i] / nrml << endl;
		out << "Node " << i << ": " << nodeBetweenness[i] / nrml << endl;
	}
	out.close();
}

int main(void)
{
	cout << "Start ok" << endl;
	int n;					// Number of nodes
	adjacency_list adjList; // Adjacency list.

	// Centrality measures.

	vector<float> nodeBetweenness;
	nodeBetweenness.resize(n, 0);

	// Input is read, and values are set to all the arguments.
	readGraph(n, adjList);

	list<int> pred[n]; // List of predecessors of node v.
	vector<int> sigma;
	vector<float> delta;
	stack<int> visitStack; // Stack that holds the inverse order of visited nodes.

	// For each node of the graph.
	for (int src = 0; src < n; src++)
	{
		// Prepare the variables for the next loop.
		resetVariables(src, n, pred, sigma, delta);

		bfs_SSSP(src, n, visitStack, sigma, pred, adjList);
		//cout<<"lopp aft ok"<<endl;
		// Get the inverse order of visited nodes.
		
		cout<<"\nsigma";
		for (auto i = sigma.begin();i!=sigma.end();i++)
		cout<<*i<<" ";
		cout<<endl;
		while (!visitStack.empty())
		{
			int w = visitStack.top();
			visitStack.pop();

			// For each predecessors of node w, do the math!
			for (list<int>::iterator it = pred[w].begin(); it != pred[w].end(); it++)
			{
				int v = *it;
				float c = ((float)sigma[v] / (float)sigma[w]) * (1.0 + delta[w]);

				delta[v] += c;
			}
			// Node betweenness aggregation part.
			if (w != src)
			{
				nodeBetweenness[w] += delta[w];
			}
		}
	}
	printNodeBetweenness(n, nodeBetweenness, true);
	return 0;
}