#ifndef POINT_H
#define POINT_H

#include <stdarg.h>
#include <vector>
#include<exception>
namespace LOF{
	class CPoint
	{
	private:
		std::vector<double> coordinate;
		int dimension;
	public:
		CPoint(int, ...);	// dimension and set of coordinates
		~CPoint();

		void SetValue(int, double);	//set nth coordinate
		double GetValue(int);	//get nth coordinate
		int GetDimension();

	};

	bool IsSame(CPoint, CPoint);	//�ж��Ƿ���ͬ

	double DistEuclidean(CPoint, CPoint);
}
#endif