#include <iostream>
#include <stdarg.h>
#include <vector>
#include <exception>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <cfloat>
#include <fstream>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include "iomanip"
#include <sstream>
#include <math.h>
#include <time.h>
#include <set>
#include <algorithm>

struct nodetree
{
	std::map<std::string, nodetree> tree = {};
};

//globals
int n_nodes;	//total number of nodes
int n_malnodes; //number of malicious nodes
std::vector<std::string> malnodlist;// = {"8", "4"};
int numtrees = 10; // num trees = length of times vector
std::vector<float> times;
std::vector<nodetree> trees;
std::map<float, float> lof;							   //outlier factor at time
std::map<std::string, std::map<float, float>> nodeinf; // inner map ->influence of node at time, outer map->node data

int realvalue = 10;

int maxval = 10;
int minval = 0;
float avgval = (maxval + minval) / 2;
float malval(float num)
{
	if (num > avgval)
		return minval;
	else
		return maxval;
}

//point class
namespace LOF
{
	class CPoint
	{
	private:
		std::vector<double> coordinate;
		int dimension;

	public:
		CPoint(int, ...); // dimension and set of coordinates
		~CPoint();

		void SetValue(int, double); //set nth coordinate
		double GetValue(int);		//get nth coordinate
		int GetDimension();
	};

	bool IsSame(CPoint, CPoint);

	double DistEuclidean(CPoint, CPoint);
}

LOF::CPoint::CPoint(int n, ...)
{
	dimension = n;
	if (!coordinate.empty())
		coordinate.clear();

	va_list ap;
	va_start(ap, n);
	double temp;
	for (int i = 0; i < n; i++)
	{
		temp = va_arg(ap, double);
		coordinate.push_back(temp);
	}
	va_end(ap);
}

LOF::CPoint::~CPoint()
{
	coordinate.clear();
}

void LOF::CPoint::SetValue(int index, double value)
{
	try
	{
		coordinate.at(index) = value;
	}
	catch (std::exception e)
	{
		printf(e.what());
		printf("\n");
		exit(0);
	}
}

double LOF::CPoint::GetValue(int index)
{
	try
	{
		return coordinate.at(index);
	}
	catch (std::exception e)
	{
		printf(e.what());
		printf("\n");
		exit(0);
	}
}

int LOF::CPoint::GetDimension() { return dimension; }

bool LOF::IsSame(CPoint point1, CPoint point2)
{
	if (point1.GetDimension() != point2.GetDimension())
		return false;
	else
	{
		int dimension = point1.GetDimension();
		for (int i = 0; i < dimension; i++)
		{
			if (point1.GetValue(i) != point2.GetValue(i))
				return false;
		}
	}
	return true;
}

double LOF::DistEuclidean(CPoint point1, CPoint point2)
{
	// check if instances are of same dimension
	if (point1.GetDimension() != point2.GetDimension())
	{
		printf("DistEuclidean error: instances have different dimensions.");
		exit(0);
	}
	else
	{
		int dimension = point1.GetDimension();
		// init differences vector
		std::vector<double> differences(dimension, 0.0);
		// compute difference for each instance and store it to differences vector
		for (int i = 0; i < dimension; i++)
		{
			differences.at(i) = (double)point1.GetValue(i) - (double)point2.GetValue(i);
		}
		// jj returning ditance instead of rmse
		double sum = 0;
		for (int i = 0; i < dimension; i++)
		{
			sum += ((double)differences.at(i) * (double)differences.at(i));
		}
		double RMSE = std::sqrt(sum);
		return RMSE;
		/// (double)dimension
	}
}

//LOF CLASS

namespace LOF
{
	class CLof
	/*	local outlier factor
		*/
	{
	public:
		//private:
		std::vector<CPoint> vec_Instances;
		std::vector<double> vec_MaxAttributeValue;
		std::vector<double> vec_MinAttributeValue;

		bool normalize;
		int n_InstanceDimension;

		void NormalizeInstances();
		CPoint NormalizeInstance(CPoint);
		void ComputeInstanceAttributeBounds();

	public:
		CLof(std::vector<CPoint>, bool);
		~CLof();

		double LocalOutlierFactor(int, CPoint);
		void debug_printout();
	};

	double k_distance(int, CPoint, std::vector<CPoint>, std::vector<CPoint> *);

	double ReachabilityDist(int, CPoint, CPoint, std::vector<CPoint>);

	double LocalReachabilityDensity(int, CPoint, std::vector<CPoint>);

	struct outlier
	{
		double lof;		 // lof
		CPoint instance; // CPoint
		int index;		 // instance
	};

	std::vector<outlier> GetOutliers(int, std::vector<CPoint>);
	std::vector<outlier> GetLOF(int, std::vector<CPoint>); //by me
}

LOF::CLof::CLof(std::vector<CPoint> instances, bool isnormalize)
{
	if (!vec_Instances.empty())
		vec_Instances.clear();
	if (!vec_MaxAttributeValue.empty())
		vec_MaxAttributeValue.clear();
	if (!vec_MinAttributeValue.empty())
		vec_MinAttributeValue.clear();

	vec_Instances = instances;
	// instance dimension
	n_InstanceDimension = vec_Instances.at(0).GetDimension();
	for (int i = 1; i < vec_Instances.size(); i++)
	{
		if (n_InstanceDimension != vec_Instances.at(i).GetDimension())
		{
			printf("class LOF init error: instances have different dimensions.");
			exit(0);
		}
	}

	normalize = isnormalize;
	if (normalize)
		NormalizeInstances();
}

LOF::CLof::~CLof()
{
	vec_Instances.clear();
	vec_MaxAttributeValue.clear();
	vec_MinAttributeValue.clear();
}

void LOF::CLof::ComputeInstanceAttributeBounds()
{
	vec_MaxAttributeValue.resize(n_InstanceDimension);
	vec_MinAttributeValue.resize(n_InstanceDimension);

	for (int i = 0; i < n_InstanceDimension; i++)
	{
		vec_MaxAttributeValue.at(i) = (double)vec_Instances.at(0).GetValue(i);
		vec_MinAttributeValue.at(i) = (double)vec_Instances.at(0).GetValue(i);
	}
	for (int j = 1; j < vec_Instances.size(); j++)
	{
		for (int i = 0; i < n_InstanceDimension; i++)
		{
			if ((double)vec_Instances.at(j).GetValue(i) > (double)vec_MaxAttributeValue.at(i))
				vec_MaxAttributeValue.at(i) = (double)vec_Instances.at(j).GetValue(i);
			if ((double)vec_Instances.at(j).GetValue(i) < (double)vec_MinAttributeValue.at(i))
				vec_MinAttributeValue.at(i) = (double)vec_Instances.at(j).GetValue(i);
		}
	}
}

void LOF::CLof::NormalizeInstances()
/*	Normalizes the instances and stores the infromation for rescaling new instances.
*/
{
	if (vec_MaxAttributeValue.size() == 0)
		this->ComputeInstanceAttributeBounds();
	for (int i = 0; i < vec_Instances.size(); i++)
		// normalize the i-th instance in instances
		vec_Instances.at(i) = this->NormalizeInstance(vec_Instances.at(i));
}

LOF::CPoint LOF::CLof::NormalizeInstance(CPoint instance)
/*	Normalize: (value - min)/(max - min)
*/
{
	CPoint normalizedInstance(n_InstanceDimension);
	double max, min, value;

	for (int i = 0; i < n_InstanceDimension; i++)
	{
		value = (double)instance.GetValue(i);
		max = (double)vec_MaxAttributeValue.at(i);
		min = (double)vec_MinAttributeValue.at(i);
		if (max - min == 0)
			normalizedInstance.SetValue(i, 0.0);
		else
		{
			double normalizedValue;
			normalizedValue = (value - min) / (max - min);
			normalizedInstance.SetValue(i, normalizedValue);
		}
	}
	return normalizedInstance;
}

double LOF::CLof::LocalOutlierFactor(int minPts, CPoint instance)
/*	The (local) outlier factor of instance captures the degree to which we call instance an outlier.
	min_pts is a parameter that is specifying a minimum number of instances to consider for computing LOF value.
*/
{
	CPoint lofInstance = instance;
	if (normalize)
		lofInstance = NormalizeInstance(instance);

	double k_distanceValue;
	std::vector<CPoint> vec_Neighbours;

	k_distanceValue = k_distance(minPts, lofInstance, this->vec_Instances, &vec_Neighbours);

	double instanceLrd = LocalReachabilityDensity(minPts, lofInstance, this->vec_Instances);

	std::vector<double> vec_lrdRatios(vec_Neighbours.size(), 0.0);

	for (int i = 0; i < vec_Neighbours.size(); i++)
	{
		std::vector<CPoint> vec_InstancesWithoutNeighbour = this->vec_Instances;

		for (int k = 0; k < vec_InstancesWithoutNeighbour.size(); k++)
		{
			for (int j = k + 1; j < vec_InstancesWithoutNeighbour.size(); j++)
			{
				if (IsSame(vec_InstancesWithoutNeighbour.at(j), vec_InstancesWithoutNeighbour.at(k)))
				{
					vec_InstancesWithoutNeighbour.erase(vec_InstancesWithoutNeighbour.begin() + j);
					j--;
				}
			}
		}

		// instances_without_neighbour.discard(neighbour) ȥ����neighbourһ����Ԫ��
		for (int j = 0; j < vec_InstancesWithoutNeighbour.size(); j++)
		{
			if (IsSame(vec_Neighbours.at(i), vec_InstancesWithoutNeighbour.at(j)))
			{
				vec_InstancesWithoutNeighbour.erase(vec_InstancesWithoutNeighbour.begin() + j);
				j--;
			}
		}

		double neighbourLrd;
		neighbourLrd = LocalReachabilityDensity(minPts, vec_Neighbours.at(i), vec_InstancesWithoutNeighbour);
		vec_lrdRatios.at(i) = (double)(neighbourLrd / instanceLrd);
	}

	double sum = 0;
	for (int i = 0; i < vec_lrdRatios.size(); i++)
		sum += (double)vec_lrdRatios.at(i);

	double lenNeighbours = (double)vec_Neighbours.size();
	return (double)(sum / lenNeighbours);
}

void LOF::CLof::debug_printout()
/*	debug
*/
{

	std::cout << "instace num: " << vec_Instances.size() << std::endl;
	std::cout << "instace dimension: " << n_InstanceDimension << std::endl;

	std::cout << "max attribute value: (";
	for (int i = 0; i < n_InstanceDimension; i++)
		std::cout << vec_MaxAttributeValue.at(i) << ", ";
	std::cout << ")" << std::endl;

	std::cout << "min attribute value: (";
	for (int i = 0; i < n_InstanceDimension; i++)
		std::cout << vec_MinAttributeValue.at(i) << ", ";
	std::cout << ")" << std::endl;

	NormalizeInstances();
	std::cout << "normalized instance[0]: (";
	for (int i = 0; i < n_InstanceDimension; i++)
		std::cout << vec_Instances.at(0).GetValue(i) << ", ";
	std::cout << ")" << std::endl;
}

double LOF::k_distance(int k, CPoint instance, std::vector<CPoint> instances, std::vector<CPoint> *pvec_Neighbours)
/*	Computes the k-distance of instance as defined in paper. 
	It also gatheres the set of k-distance neighbours.
*/
{
	// distances:( key: distance_value -> value: instances )
	std::map<double, std::vector<CPoint>> map_Distances;
	std::map<double, std::vector<CPoint>>::iterator iter;

	double distanceValue;

	for (int i = 0; i < instances.size(); i++)
	{
		distanceValue = DistEuclidean(instance, instances.at(i));
		iter = map_Distances.find(distanceValue);
		if (iter != map_Distances.end()) // find
		{
			map_Distances[distanceValue].push_back(instances.at(i));
		}
		else // not find
		{
			std::vector<CPoint> vec_temp;
			vec_temp.push_back(instances.at(i));
			// map key
			map_Distances.insert(std::pair<double, std::vector<CPoint>>(distanceValue, vec_temp));
		}
	}

	int k_sero = 0;
	double k_dist;
	pvec_Neighbours->clear();

	// neighbours kinstance
	for (iter = map_Distances.begin(); iter != map_Distances.end(); ++iter)
	{
		k_sero += iter->second.size();
		for (int i = 0; i < iter->second.size(); i++)
		{
			pvec_Neighbours->push_back(iter->second.at(i));
		}
		k_dist = (double)iter->first;
		if (k_sero >= k)
			break;
	}

	/*	// print
	for (iter = map_Distances.begin(); iter != map_Distances.end();++iter)
	{
		std::cout << iter->first << std::endl;
		for (int i = 0; i < iter->second.size(); i++)
		{
			for (int j = 0; j < iter->second.at(i).GetDimension(); j++)
			{
				std::cout << iter->second.at(i).GetValue(j) << " ";
			}
			std::cout << std::endl;
		}
	}*/
	return k_dist;
}

double LOF::ReachabilityDist(int k, CPoint instance1, CPoint instance2, std::vector<CPoint> vec_Instances)
/*	The reachability distance of instance1 with respect to instance2.
*/
{
	double k_distanceValue;
	std::vector<CPoint> neighbours;

	k_distanceValue = k_distance(k, instance2, vec_Instances, &neighbours);

	double distanceEuclidean = DistEuclidean(instance1, instance2);

	if ((double)k_distanceValue > (double)distanceEuclidean)
		return k_distanceValue;
	else
		return distanceEuclidean;
}

double LOF::LocalReachabilityDensity(int minPts, CPoint instance, std::vector<CPoint> vec_Instances)
/*	Local reachability density of instance is the inverse of the average reachability 
    distance based on the min_pts-nearest neighbors of instance.
*/
{
	double k_distanceValue;
	std::vector<CPoint> vec_Neighbours;

	k_distanceValue = k_distance(minPts, instance, vec_Instances, &vec_Neighbours);

	std::vector<double> vec_ReachabilityDistances(vec_Neighbours.size(), 0); // n.zeros(len(neighbours))

	for (int i = 0; i < vec_Neighbours.size(); i++)
	{
		vec_ReachabilityDistances.at(i) = ReachabilityDist(minPts, instance, vec_Neighbours.at(i), vec_Instances);
	}

	double sumReachDist = 0.0;
	for (int i = 0; i < vec_ReachabilityDistances.size(); i++)
		sumReachDist += (double)vec_ReachabilityDistances.at(i);

	if (sumReachDist == 0.0)
		return DBL_MAX; // return inf
	double lenNeighbour = (double)vec_Neighbours.size();
	return (double)(lenNeighbour / sumReachDist);
}

std::vector<LOF::outlier> LOF::GetOutliers(int k, std::vector<LOF::CPoint> vec_Instances)
/* outliers Ϊvector<struct outlier>
	outlier{
	double lof;
	CPoint instance;
	int index;}
*/
{
	std::vector<CPoint> vec_InstancesBackUp;
	std::vector<outlier> vec_Outliers;
	vec_Outliers.clear();

	for (int i = 0; i < vec_Instances.size(); i++)
	{
		CPoint instance = vec_Instances.at(i);
		vec_InstancesBackUp = vec_Instances;
		vec_InstancesBackUp.erase(vec_InstancesBackUp.begin() + i);
		CLof l(vec_InstancesBackUp, false);
		double value = l.LocalOutlierFactor(k, instance);
		std::cout << "outval : " << instance.GetValue(1) << " : " << value << std::endl;
		if (value > 1.0)
		{
			outlier temp = {value, instance, i}; // { lof, instance, index }
			vec_Outliers.push_back(temp);
		}
	}

	//
	for (int i = 0; i < vec_Outliers.size(); i++)
	{
		for (int j = 0; j < vec_Outliers.size() - 1 - i; j++)
		{
			if (vec_Outliers.at(j).lof < vec_Outliers.at(j + 1).lof)
			{
				outlier temp = vec_Outliers.at(j + 1);
				vec_Outliers.at(j + 1) = vec_Outliers.at(j);
				vec_Outliers.at(j) = temp;
			}
		}
	}

	return vec_Outliers;
}

std::vector<LOF::outlier> LOF::GetLOF(int k, std::vector<LOF::CPoint> vec_Instances)
{
	std::vector<CPoint> vec_InstancesBackUp;
	std::vector<outlier> vec_Outliers;
	std::vector<outlier> vec_lof;
	vec_lof.clear();

	for (int i = 0; i < vec_Instances.size(); i++)
	{
		CPoint instance = vec_Instances.at(i);
		vec_InstancesBackUp = vec_Instances;
		vec_InstancesBackUp.erase(vec_InstancesBackUp.begin() + i);
		CLof l(vec_InstancesBackUp, false);
		double value = l.LocalOutlierFactor(k, instance);

		outlier temp = {value, instance, i}; // { lof, instance, index }
		vec_Outliers.push_back(temp);
	}

	return vec_Outliers;
}

void vlofanomaly()
{
}

//connect nodes to form trees
void gentrees()
{
	srand(time(0));
	for (int i = 0; i < numtrees; i++)
	{
		std::cout << "\n\n\ntree " << i << std::endl;
		std::vector<std::string> nodeids;
		nodetree currtree;	// the tree that holds all nodes and shrinks to one tree
		nodetree treecache; //to hold trees in vector pop etc

		for (int j = 0; j < n_nodes; j++)
		{
			nodeids.push_back("" + std::to_string(j));
		}
		for (int j = 0; j < n_nodes; j++)
		{
			currtree.tree[nodeids[j]] = {};
		}

		std::vector<std::string> nodeids2(nodeids);
		std::vector<std::string> SenderNodesStack; //stack formed in the order that nodes send data, stores node index from nodeids
		for (int j = 0; j < n_nodes; j++)
		{
			int v = rand() % nodeids2.size();
			//std::cout << v<<" ";
			SenderNodesStack.push_back(nodeids2[v]);
			nodeids2.erase(nodeids2.begin() + v);
		}

		//std::vector<std::string> nodeids3(SenderNodesStack);//recivers
		while (!SenderNodesStack.empty())
		{
			std::string sendernode = SenderNodesStack.back();
			SenderNodesStack.pop_back();

			treecache = currtree.tree[sendernode];

			if (!SenderNodesStack.empty())
			{
				int v = rand() % SenderNodesStack.size(); //random reciver

				std::string receiver = SenderNodesStack[v];

				currtree.tree[receiver].tree[sendernode] = treecache;
				std::cout << sendernode << " -> " << receiver << std::endl;
				currtree.tree.erase(sendernode);
			}
		}

		trees.push_back(currtree);
	}
}
std::string genNewickTree(nodetree t) //for one tree
{
	if (t.tree.size())
	{
		int childnum = t.tree.size();
		std::string st = "(";
		int pos = 1;
		for (auto i : t.tree)
		{
			st += i.first + genNewickTree(i.second);
			if (pos != t.tree.size())
				st += ",";
			pos++;
		}
		st += ")";
		return st;
	}
	return ""; //return empty string for leaf node
}
void getNewickTrees()
{
	std::ofstream fout;
	fout.open("trees.txt", std::ofstream::trunc); //truncate brfore filling
	for (int i = 0; i < n_nodes; i++)
	{
		std::string st = genNewickTree(trees[i]);
		std::cout << "\n\n\ntree " << i << "\n"
				  << st << std::endl;
		fout << st << std::endl;
	}
	fout.close();
}

std::pair<float, int> SimDataTransmit(nodetree t) //for one tree
{
	if (t.tree.size())
	{
		float s = 0;
		int n = 0;
		std::pair<float, int> rsrn;
		for (auto i : t.tree)
		{

			if (std::find(malnodlist.begin(), malnodlist.end(), i.first) == malnodlist.end()) //not malicious
			{
				rsrn = SimDataTransmit(i.second);
				s += (rsrn.first + realvalue) / (rsrn.second + 1);
			}
			else
				s += malval(realvalue);
			n++;
		}

		return std::make_pair(s, n);
	}
	return std::make_pair(0.0, 0);
}

void GetallAggregates()
{
	std::ofstream fout;
	fout.open("inpagg.txt", std::ofstream::trunc);
	std::pair<float, int> rsrn;
	//std::cout<<numtrees<<std::endl;
	std::cout << "\n\nAggregate values at time\n\n";
	for (int i = 0; i < numtrees; i++)
	{
		rsrn = SimDataTransmit(trees[i]);
		std::cout << times[i] << "\t" << rsrn.first / rsrn.second << "\n";
		fout << times[i] << "\t" << rsrn.first / rsrn.second << "\n";
	}
	fout.close();
}

void treeinf(float currtime)
{
	std::ifstream fp;
	fp.open("inpdata.txt");

	// class tree
	// {
	// 	map<string,tree> node;
	// };
	std::map<std::string, std::vector<std::string>> tree_child_list;
	std::stack<std::string> stk;
	std::string buffer = "root";
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

	std::string root = tree_child_list["root"][0];
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
	std::map<std::string, float> influence;

	influence[root] = 1;
	std::queue<std::string> q;
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
		std::cout << i.first << "\t" << i.second << std::endl;
	}
	for (auto i : influence)
	{
		nodeinf[i.first][currtime] = i.second;
	}
}

void dataoutlierfactor()
{
	std::ifstream fp;
	fp.open("inpagg.txt");
	std::string line;
	double timeval, agg;
	std::vector<LOF::CPoint> instances;
	while (getline(fp, line))
	{
		std::istringstream iss(line);
		iss >> timeval >> agg;
		instances.push_back(LOF::CPoint(2, timeval, agg));
	}
	fp.close();

	std::vector<LOF::outlier> out;
	out = GetLOF(5, instances);

	std::cout << "\n\nLocal outlier factor at time\n";
	for (int i = 0; i < out.size(); i++)
	{
		std::cout << out.at(i).instance.GetValue(0) << "\t";
		std::cout << out.at(i).lof << std::endl;
		lof[out.at(i).instance.GetValue(0)] = out.at(i).lof;
	}
}
void pearsonnormalize()
{
	float omin = 100, omax = 0;
	float nmin = 0,nmax = 1;
	for (auto i : lof)
	{
		if (i.second > omax)
			omax = i.second;
		if (i.second < omin)
			omin = i.second;
	}
	for(auto i:lof)
	{
		float x = i.second;
		i.second = ((x-omin)/(omax-omin))*(nmax-nmin) + nmin;
	}
}
void correlate()
{
	// std::ifstream nodeinfdata, outlierdata;
	// nodeinfdata.open("corinfluencetable.txt");
	// outlierdata.open("corlofoutput.txt");

	// std::map<float, float> lof;							   //outlier factor at times
	// std::map<std::string, std::map<float, float>> nodeinf; // inner map ->influence of node at times, outer map->node data

	// std::vector<float> times; //store the times line(first line) from file

	// std::string line;

	// getline(nodeinfdata, line); //read the times line(first line) from file
	// std::istringstream iss(line);
	// float t;
	// while (iss >> t)
	// 	times.push_back(t);
	// std::string node;
	// while (getline(nodeinfdata, line))
	// {
	// 	std::istringstream iss(line);
	// 	iss >> node;
	// 	for (auto i : times)
	// 		iss >> nodeinf[node][i];
	// }
	// nodeinfdata.close();

	// float tim, tlof;
	// while (getline(outlierdata, line))
	// {
	// 	std::istringstream iss(line);
	// 	iss >> tim >> tlof;
	// 	lof[tim] = tlof;
	// }
	// outlierdata.close();

	std::cout << "node influence table\n";
	for (auto i : nodeinf)
	{
		std::cout << i.first << "\t";
		for (auto j : i.second)
		{
			std::cout << j.second << "\t";
		}
		std::cout << std::endl;
	}

	std::cout << "\nlof at times\n";

	for (auto i : lof)
	{
		std::cout << i.first << " : " << i.second << std::endl;
	}

	std::map<std::string, float> correlation;
	int pi = times.size();
	// for (auto i : nodeinf)
	// {
	// 	correlation[i.first] = 1;
	// 	float sum = 0;
	// 	for (auto j : times)
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
			correlation[i.first] = 0;
			for (auto j : times)
				correlation[i.first] += (lof[j] - xavg) * (nodeinf[i.first][j] - yavg);
			correlation[i.first] /= denom;
		}
	}

	std::cout << "correlation data\n";
	int max = -1000;
	std::string malnodid;
	for (auto i : correlation)
	{
		if(i.second>max)
		{
			max=i.second;
			malnodid = i.first;
		}
		std::cout << i.first << " : " << i.second << std::endl;
	}

	//std::cout<<"\nSuspicious node is "<<malnodid<<"\n";
}
int main()
{
	std::string mnode;
	std::cout << "No of nodes : ";
	std::cin >> n_nodes;
	std::cout << "No of malicious nodes : ";
	std::cin >> n_malnodes;
	for(int i=0;i<n_malnodes;i++)
	{
		std::cout<<"\nEnter node id : ";
		std::cin>>mnode;
		malnodlist.push_back(mnode);
	}

	for (int i = 0; i < numtrees; i++)
	{
		times.push_back((i + 1.0) / 10);
	}
	gentrees();
	getNewickTrees();

	std::ifstream fin;
	fin.open("trees.txt");
	for (int i = 0; i < n_nodes; i++)
	{
		std::string st;
		fin >> st;
		std::ofstream fout;
		fout.open("inpdata.txt", std::ofstream::trunc);
		fout << st;
		fout.close();

		std::cout << "\n\ntime " << times[i] << "\n";
		treeinf(times[i]);
	}
	GetallAggregates();
	dataoutlierfactor();
	correlate();
	//call transmit
	//call treeinf in a loop and append infulence to generate influence table
	return 0;
}