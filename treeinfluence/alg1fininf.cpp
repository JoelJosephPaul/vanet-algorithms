//this version calculates the nodes influence on final the
// aggregate if it was malicious
// depends on number of siblings and influence of parent, not on number of children
// tree must be represented in this program :(
//to do, input file must contain time data too for tree

#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

int main()
{
	ifstream fp;
	fp.open("inpdata.txt");

	// class tree
	// {
	// 	map<string,tree> node;
	// };
	map<string, vector<string>> tree_child_list;
	stack<string> stk;
	string buffer = "root";
	char c = '_', prevc;

	prevc = c;
	fp >> c;

	while (!fp.eof())
	{
		switch (c)
		{
		case ',':
			tree_child_list[stk.top()].push_back(buffer); //adding chilren to parent
			if (prevc != ')')
				tree_child_list[buffer] = {}; //no further children
			buffer = "";
			break;
		case '(':
			stk.push(buffer);
			buffer = "";
			break;
		case ')':										  //this is the last comma child counter
			tree_child_list[stk.top()].push_back(buffer); //adding chilren to parent
			if (prevc != ')')
				tree_child_list[buffer] = {}; //no further children
											  /*
			child_num = stk.top().second;
			influence[stk.top().first] = child_num + 1; //+1 to indicate influence of self
			stk.pop();
			stk.top().second += child_num;*/

			buffer = stk.top();
			stk.pop();
			break;
		default:
			buffer += c;
		}
		prevc = c;
		fp >> c;
	}

	fp.close();

	//assuming tree reading over

	//not reading tree now, constructing final result
	/*
	tree_child_list["9"] = {"3", "8"};
	tree_child_list["3"] = {"0", "1", "2"};
	tree_child_list["8"] = {"6", "7"};
	tree_child_list["6"] = {"4", "5"};
	tree_child_list["0"] = {};
	tree_child_list["1"] = {};
	tree_child_list["2"] = {};
	tree_child_list["4"] = {};
	tree_child_list["5"] = {};
	tree_child_list["7"] = {};

	string root = "9";*/
	string root = tree_child_list["root"][0];
/*
	cout << "the tree" << endl;
	for (auto i : tree_child_list)
	{
		cout << i.first << " : ";
		for (auto j : i.second)
		{
			cout << j << " ";
		}
		cout << endl;
	}
	cout<<"root : "<<root<<endl;*/
	map<string, float> influence;

	influence[root] = 1;
	queue<string> q;
	q.push(root);
	float newinf;
	while (q.size())
	{
		root = q.front();
		newinf = influence[root] / (tree_child_list[root].size() + 1);
		for (auto i : tree_child_list[root])
		{
			q.push(i);
			influence[i] = newinf;
		}
		q.pop();
	}

	for (auto i : influence)
	{
		cout << i.first << " : " << i.second << endl;
	}

	return 0;
}