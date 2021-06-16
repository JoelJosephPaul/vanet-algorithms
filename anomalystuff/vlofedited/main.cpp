#include <iostream>
#include "iomanip"
#include <vector>
#include "point.h"
#include "lof.h"
#include <fstream>

using namespace std;
using namespace LOF;

int main()
{
	//task now
	//point class does not have ID. should it? based on time?
	//restructre all class to hav necessary code. remove unnecessary classes, functionality, func calls etc
	//check to remove data duplication etc
	//try reading from file
	ifstream fp;
	fp.open("inpagg.txt");
	string line;
	double time,agg;
	vector<CPoint> instances;
	while(getline(fp,line))
	{
		istringstream iss(line);
		iss>>time>>agg;
		instances.push_back(CPoint(2, time, agg));
	}
	fp.close();

	vector<outlier> out;
	out = GetLOF(5, instances);

	for (int i = 0; i < out.size(); i++)
	{
		cout << out.at(i).instance.GetValue(0) << "\t";
		cout << out.at(i).lof << endl;
	}

	return 0;
}