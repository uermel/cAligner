#ifndef FILTERSIZE3D_H
#define FILTERSIZE3D_H

#include "FilterROI3D.h"

class FilterSize3D
{
public:
	int width;
	int height;
	int depth;

	FilterSize3D();
	FilterSize3D(int aWidth, int aHeight, int aDepth);
	FilterSize3D(const FilterSize3D& aSize);


	FilterSize3D& operator=(const FilterSize3D& arg);
	bool operator==(const FilterSize3D& arg);

	FilterSize3D operator+(int value) const;
	FilterSize3D operator+(const FilterSize3D& value) const;

	FilterSize3D& operator+=(int value);
	FilterSize3D& operator+=(const FilterSize3D& value);

	FilterSize3D operator-(int value) const;
	FilterSize3D operator-(const FilterSize3D& value) const;

	FilterSize3D& operator-=(int value);
	FilterSize3D& operator-=(const FilterSize3D& value);

	FilterSize3D operator*(int value) const;
	FilterSize3D operator*(const FilterSize3D& value) const;

	FilterSize3D& operator*=(int value);
	FilterSize3D& operator*=(const FilterSize3D& value);

	FilterSize3D operator/(int value) const;
	FilterSize3D operator/(const FilterSize3D& value) const;

	FilterSize3D& operator/=(int value);
	FilterSize3D& operator/=(const FilterSize3D& value);

	FilterROI3D operator+(const FilterROI3D& value) const;
	FilterROI3D operator-(const FilterROI3D& value) const;
	FilterROI3D operator*(const FilterROI3D& value) const;
	FilterROI3D operator/(const FilterROI3D& value) const;
};


FilterSize3D operator+(int lhs, const FilterSize3D& rhs);
FilterSize3D operator-(int lhs, const FilterSize3D& rhs);
FilterSize3D operator*(int lhs, const FilterSize3D& rhs);
FilterSize3D operator/(int lhs, const FilterSize3D& rhs);

#endif