#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <algorithm>
using namespace std;
//to do it as in paper, influence of a node is number of children(if in tree form)
// do postorder traversal
int main()
{
	string buffer;
	ifstream fp;
	fp.open("ipdata.txt");
	// 	, ( ) eof
	int l = 1, n = 0;
	char c;
	map<string, float> influence;
	int mval = 0;
	while (!fp.eof())
	{
		fp >> c;
		switch (c)
		{
		case ',':
			if (buffer.size())
				influence[buffer] = l;
			buffer = "";
			break;
		case '(':
			if (buffer.size())
				influence[buffer] = l;
			buffer = "";
			l++;
			break;
		case ')':
			if (buffer.size())
				influence[buffer] = l;
			buffer = "";
			l--;
			break;
		default:
			buffer += c;
		}
		if (l > mval)
			mval = l;
	}
	fp.close();
	float var = 1.0 / mval;
	cout << "var " << var << endl;
	for (auto &i : influence)
		i.second = 1 - (i.second - 1) * var;
	for (auto i : influence)
	{
		cout << i.first << " : " << i.second << endl;
	}
	return 0;
}
