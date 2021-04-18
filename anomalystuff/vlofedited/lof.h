#ifndef LOF_H
#define LOF_H

#include <math.h>
#include "point.h"
#include <iostream>
#include <map>
#include <iterator>

namespace LOF{
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

	double k_distance(int, CPoint, std::vector<CPoint>, std::vector<CPoint>*);

	double ReachabilityDist(int, CPoint, CPoint, std::vector<CPoint>);

	double LocalReachabilityDensity(int, CPoint, std::vector<CPoint>);

	struct outlier
	{
		double lof;			// lof��ֵ
		CPoint instance;	// CPoint���������Ϣ
		int index;			// ��instance���±�
	};

	std::vector<outlier> GetOutliers(int, std::vector<CPoint>);
	std::vector<outlier> GetLOF(int, std::vector<CPoint>);//by me
}
#endif