//generate correlation of each node
//generate list of suspicious nodes
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <math.h>
using namespace std;

int main()
{
	ifstream nodeinfdata, outlierdata;
	nodeinfdata.open("corinfluencetable.txt");
	outlierdata.open("corlofoutput.txt");

	map<float, float> lof;					//outlier factor at time
	map<string, map<float, float>> nodeinf; // inner map ->influence of node at time, outer map->node data

	vector<float> time; //store the time line(first line) from file

	string line;

	getline(nodeinfdata, line); //read the time line(first line) from file
	istringstream iss(line);
	float t;
	while (iss >> t)
		time.push_back(t);
	string node;
	while (getline(nodeinfdata, line))
	{
		istringstream iss(line);
		iss >> node;
		for (auto i : time)
			iss >> nodeinf[node][i];
	}
	nodeinfdata.close();

	float tim, tlof;
	while (getline(outlierdata, line))
	{
		istringstream iss(line);
		iss >> tim >> tlof;
		lof[tim] = tlof;
	}
	outlierdata.close();

	cout << "node influence table\n";
	for (auto i : nodeinf)
	{
		cout << i.first << "\t";
		for (auto j : i.second)
		{
			cout << j.second << "\t";
		}
		cout << endl;
	}

	cout << "\nlof at time\n";

	for (auto i : lof)
	{
		cout << i.first << " : " << i.second << endl;
	}

	map<string, float> correlation;
	int pi = time.size();
	// for (auto i : nodeinf)
	// {
	// 	correlation[i.first] = 1;
	// 	float sum = 0;
	// 	for (auto j : time)
	// 		sum += (lof[j] - nodeinf[i.first][j]) * (lof[j] - nodeinf[i.first][j]);
	// 	//cout<<sum<<endl;
	// 	correlation[i.first] -= 6 * sum / (pi * (pi - 1));
	// }

	float xavg = 0; //lof
	for (auto k : lof)
	{
		xavg += k.second;
	}
	xavg /= pi;
	float sdx = 0; //standard deviation
	for (auto k : lof)
	{
		sdx += (k.second - xavg) * (k.second - xavg);
	}
	sdx = sqrt(sdx);

	for (auto i : nodeinf)
	{
		float yavg = 0; //influence
		for (auto k : i.second)
		{
			yavg += k.second;
		}
		yavg /= pi;

		float sdy = 0; //standard deviation
		for (auto k : i.second)
		{
			sdy += (k.second - yavg) * (k.second - yavg);
		}
		sdy = sqrt(sdy);
		float denom = sdx * sdy;
		if (denom == 0)
			correlation[i.first] = 0;
		else
		{
			correlation[i.first]=0;
			for(auto j:time)
			correlation[i.first] += (lof[j]-xavg)*(nodeinf[i.first][j]-yavg);
			correlation[i.first]/=denom;
		}
	}

	cout << "correlation data\n";
	for (auto i : correlation)
		cout << i.first << " : " << i.second << endl;
	return 0;
}