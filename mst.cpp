#include "mst.h"

void Error::invalidParam(int in_num)
{
	if(in_num < 0)
		throw -1;
}

void Error::invalidParam(int in_u, int in_num)
{
    if((in_u >= in_num ) || (in_u < 0))
        throw -1;
}

void Error::invalidParam(int in_u, int in_v, int in_num)
{
    if((in_u >= in_num) || (in_v >= in_num) || (in_u < 0) || (in_u == in_v) || (in_v < 0))
        throw -1;
}

void Error::invalidParam(int in_u, int in_v, double in_w, int in_num)
{
    if((in_u >= in_num) || (in_v >= in_num) || (in_u < 0) || (in_v < 0) || (in_u == in_v) || (in_w < 0))
		throw -1;
}

Edge::Edge()
{
    src = 0;
    dest = 0;
    weight = 0;
    exist = false;
}

Edge::Edge(int in_src, int in_dest, double in_weight)
{
    src = in_src;
    dest = in_dest;
    weight = in_weight;
    exist = true;
}

Disjoint::Disjoint(int in_num)
{
    rank = new int[in_num];
    rep = new int[in_num];
    num = new int[in_num];
 
 	for(int i = 0; i < in_num; i++)
    {
        rank[i] = 0;
        rep[i] = i;
        num[i] = 1;
    }

    num_vertices = in_num;
}

Disjoint::~Disjoint()
{
    delete rank;
    rank = nullptr;
    delete rep;
    rep = nullptr;
}

//Implementing union by rank
//Returns a boolean for if sets have been joined
//Joined = true
//Skipped = false
bool Disjoint::joinSets(int in_x, int in_y)
{
    int x_rep = findSet(in_x);
    int y_rep = findSet(in_y);
  	
	//Ignores when representatives are the same because  joining will result in a loop.	
    if(x_rep != y_rep)
    {
		//Joining sets with same rank will increment the rank.
		//Joining the sets with lower rank to the higher rank(does not increase rank)
        if(rank[x_rep] >= rank[y_rep])
        {
           	if(rank[x_rep] == rank[y_rep])
            	rank[x_rep] ++;
            
            rep[y_rep] = x_rep;
            num[x_rep] += num[y_rep];
        }
        else if(rank[x_rep] < rank[y_rep])
        {
            rep[x_rep] = y_rep;
            num[y_rep] += num[x_rep];
        }
        return true;
    }
  	return false;
}

//Implements path compression
int Disjoint::findSet(int in_x)
{
    if(rep[in_x] == in_x)
        return in_x;
    else
    {
        int result = findSet(rep[in_x]);
		//Updates representative to the topmost one.
        rep[in_x] = result;
        return result;
    }
}

Graph::Graph()
{
	num_nodes = 0;
    edge_count = 0;
	adjacency = nullptr;
    degree = nullptr;
}

Graph::~Graph()
{
    for(int i = 0; i < num_nodes; ++i)
        delete adjacency[i];
    delete adjacency;
}

string Graph::setGraph(int in_num)
{
	try
	{
		Error::invalidParam(in_num);
    	num_nodes = in_num;
    	adjacency = new Edge * [in_num];
    	degree = new int [in_num];

		//Initialization
    	for(int i = 0; i < in_num; ++i)
    	{
        	adjacency[i] = new Edge[in_num];
        	degree[i] = 0;
    	}
		return "success";
	}
	catch(int e)
	{
		return "failure";
	}
}

string Graph::addEdge(int in_u, int in_v, double in_w)
{
    try
    {
        Error::invalidParam(in_u, in_v, in_w, num_nodes);
    
		//Forced u to be the smaller vertex and v to be the bigger one so only half of matrix is used(because graph is undirected)
		int u = min(in_u, in_v);
        int v = max(in_u, in_v);
        
		if(adjacency[u][v].exist == false)
        {
            adjacency[u][v].exist = true;
            adjacency[u][v].src = in_u;
            adjacency[u][v].dest = in_v;
            adjacency[u][v].weight = in_w;
            edge_count ++;
            degree[in_u] ++;
            degree[in_v] ++;
        }
        else									//Updates weight if edge exists
            adjacency[u][v].weight = in_w;
        
        return "success";
    }
    catch (int e)
    {
        return "failure";
    }
}

string Graph::delEdge(int in_u, int in_v)
{
    try
    {
        Error::invalidParam(in_u, in_v, num_nodes);
        int u = min(in_u, in_v);
        int v = max(in_u, in_v);
	
        if(adjacency[u][v].exist == false)
            return "failure";
        else
        {
            adjacency[u][v].exist = false;
            adjacency[u][v].src = 0;
            adjacency[u][v].dest = 0;
            adjacency[u][v].weight = 0;
            edge_count --;
            degree[in_u] --;
            degree[in_v] --;
            return "success";
        }
    }
    catch (int e)
    {
        return "failure";
    }
}

int Graph::getDegree(int in_u)
{
    try
    {
        Error::invalidParam(in_u, num_nodes);
		return degree[in_u];
    } catch (int e)
    {
        return -1; 
    }
}

int Graph::getEdgeCount()
{
    return edge_count;
}

string Graph::clear()
{
    for(int i = 0; i < num_nodes; ++i)
    {
        delete adjacency[i];
        adjacency[i] = new Edge[num_nodes];
        degree[i] = 0;
        edge_count = 0;
    }
	edges.clear();
    return "success";
}

//Adds all existing edges to a vector
//Used for MST function
void Graph::pushEdges()
{
	for(int i = 0; i < num_nodes; i++)
    {
        for(int j = i; j < num_nodes; j++)
        {
            if(adjacency[i][j].exist)
            {
                edges.push_back(adjacency[i][j]);
            }
        }
    }
}

double Graph::mst()
{
	//Checks if graph size is 1, outputs failure if so. 
	if(num_nodes == 1)
		return -1;

	//Initializes all disjoint sets
    Disjoint dj(num_nodes);
    double total{0};
 
	//Sorting edges by weight
    sort(edges.begin(), edges.end());
    
	//Iterate through sorted edges and calculates the weight 
    for(int i = 0; i < edge_count; i++)
    {
        bool is_joined = dj.joinSets(edges[i].src, edges[i].dest);

		if(is_joined)
            total += edges[i].weight;
    }
	
	edges.clear();	

	//Checks if MST is connected
	//If all vertices are connected, then the number of vertices of the representative should equal to total number of nodes
	//Since technically all vertices have the same representative if connected, it doesn't matter which representative is checked
    //return -1 functions as failure
	if(dj.num[dj.findSet(0)] == num_nodes)
        return total;
    else
        return -1;
}

