#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::sort;
using std::vector;
using std::min;
using std::max;
using std::stringstream;
using std::stoi;
using std::stod;

class Edge
{
public:
    Edge();
    Edge(int in_src, int in_dest, double in_w);
    bool operator<(const Edge& in_x) const
    {
        return weight < in_x.weight;
    }
private:
    int src;
    int dest;
    double weight;
    bool exist;
    friend class Graph;
};

class Disjoint
{
public:
    Disjoint(int in_num);
    ~Disjoint();
    bool joinSets(int in_x, int in_y);
    int findSet(int in_x);
private:
    int * rank;
    int * rep;
    int * num;
    int num_vertices;
    friend class Graph;
};

class Graph
{
public:
    Graph();
    ~Graph();
    string setGraph(int in_num);
	string addEdge(int in_u, int in_v, double in_weight);
    string delEdge(int in_u, int in_v);
    int getDegree(int in_u);
    int getEdgeCount();
    string clear();
    double mst();
	void pushEdges();
private:
    int num_nodes;
    int edge_count;
    int * degree;
    vector<Edge> edges;
    Edge** adjacency;
};

class Error
{
public:
	static void invalidParam(int in_num);
    static void invalidParam(int in_u, int in_num);
    static void invalidParam(int in_u, int in_v, int in_num);
    static void invalidParam(int in_u, int in_v, double in_w, int in_num);
};

