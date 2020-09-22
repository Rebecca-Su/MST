#include "mst.h"

int main();

int main()
{
	Graph g;
	string thisLine;
	string command;
	string param;
	string in_attr;
	size_t pos;
	vector<string> info;

	while(!cin.eof())
	{
		getline(cin, thisLine);
		pos = thisLine.find(" ");

		if(pos == string::npos)
			command = thisLine;
		else
		{
			command = thisLine.substr(0, pos);
			param = thisLine.substr(pos + 1, thisLine.size());
			
			stringstream ss(param);
			
			while(ss.good())
			{
				getline(ss, in_attr, ';');
				info.push_back(in_attr);
			}
		}
		
		//info[0] = u
		//info[1] = v
		//info[2] = weight	
		if(command == "n")
		{
			string result = g.setGraph(stoi(info[0]));
			cout << result << endl;
			
			if(result == "failure")
				break;
		}
		else if(command == "i")
			cout << g.addEdge(stoi(info[0]), stoi(info[1]), stod(info[2])) << endl;
		else if(command == "d")
			cout << g.delEdge(stoi(info[0]), stoi(info[1])) << endl;
		else if(command == "degree")
		{
			int result = g.getDegree(stoi(info[0]));
			
			if(result == -1)
				cout << "failure" << endl;
			else
				cout << "degree of " << stoi(info[0]) << " is " << result << endl;
		}
		else if(command == "edge_count")
			cout << "edge count is " << g.getEdgeCount() << endl;
		else if(command == "clear")
			cout << g.clear() << endl;
		else if(command == "mst")
		{
			g.pushEdges();
			double result = g.mst();
			
			if(result == -1)
				cout << "not connected" << endl;
			else
				cout << "mst " << result << endl;
		}

		info.clear();
	}
	return 0;
}
