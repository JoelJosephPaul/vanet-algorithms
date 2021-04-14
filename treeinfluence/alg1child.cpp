#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stack>
using namespace std;
//using stack
// inf of each node = total no of allchilds div m , m total nodes
int main()
{
	string buffer = "temp2";
	ifstream fp;
	stack<pair<string, int>> stk;
	fp.open("inpdata.txt");
	// 	, ( ) eof
	int n = 0;
	char c = '_', prevc;

	unordered_map<string, float> influence;
	int child_num = 0;
	//fp >> prevc; //pre reading initial ( for timebeing
	stk.push(pair<string, int>("temp1", 0));

	//cout<<"stack top now "<<stk.top().first<<":"<<stk.top().second<<" size "<<stk.size()<<endl;
	//cout<<c<<" case ( \n";

	//cin.eof will casue last character case to repeat twice
	//so read before checking eof
	prevc = c;
	fp >> c;
	while (!fp.eof())
	{
		switch (c)
		{
		case ',':
			stk.top().second++;
			if (prevc != ')')
				influence[buffer] = 1; //1 to indicate influence of self

			buffer = "";
			break;
		case '(':
			stk.push(pair<string, int>(buffer, 0));
			buffer = "";
			break;
		case ')': //this is the last comma child counter
			stk.top().second++;
			if (prevc != ')')
				influence[buffer] = 1; //1 to indicate influence of self

			child_num = stk.top().second;
			influence[stk.top().first] = child_num + 1; //+1 to indicate influence of self
			stk.pop();
			stk.top().second += child_num;

			buffer = "";
			break;
		default:
			buffer += c;
		}
		prevc = c;
		fp >> c;
	}
	fp.close();
	influence.erase("temp2");
	n = stk.top().second;
	cout << "nodes " << n << endl;
	for (auto &i : influence)
		i.second /= n;
	for (auto i : influence)
	{
		cout << i.first << " : " << i.second << endl;
	}
	return 0;
}