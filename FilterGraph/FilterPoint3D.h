#ifndef FILTERPOINT3D_H
#define FILTERPOINT3D_H

#include "../Basics/Default.h"

//Forward declatation
class FilterSize3D;
class FilterROI3D;

class FilterPoint3D
{
public:
	int x;
	int y;
	int z;

	FilterPoint3D();
	FilterPoint3D(int aX, int aY, int z);
	FilterPoint3D(const FilterPoint3D& aPoint);

	FilterPoint3D& operator=(const FilterPoint3D& arg);
	bool operator==(const FilterPoint3D& arg);

	FilterPoint3D operator+(const FilterPoint3D& value) const;
	FilterPoint3D operator+(const FilterSize3D& value) const;
	FilterPoint3D operator+(int value) const;

	FilterPoint3D operator+=(const FilterPoint3D& value);
	FilterPoint3D operator+=(const FilterSize3D& value);
	FilterPoint3D operator+=(int value);

	FilterPoint3D operator-(const FilterPoint3D& value) const;
	FilterPoint3D operator-(const FilterSize3D& value) const;
	FilterPoint3D operator-(int value) const;

	FilterPoint3D operator-=(const FilterPoint3D& value);
	FilterPoint3D operator-=(const FilterSize3D& value);
	FilterPoint3D operator-=(int value);

	FilterPoint3D operator*(const FilterPoint3D& value) const;
	FilterPoint3D operator*(const FilterSize3D& value) const;
	FilterPoint3D operator*(int value) const;

	FilterPoint3D operator*=(const FilterPoint3D& value);
	FilterPoint3D operator*=(const FilterSize3D& value);
	FilterPoint3D operator*=(int value);

	FilterPoint3D operator/(const FilterPoint3D& value) const;
	FilterPoint3D operator/(const FilterSize3D& value) const;
	FilterPoint3D operator/(int value) const;

	FilterPoint3D operator/=(const FilterPoint3D& value);
	FilterPoint3D operator/=(const FilterSize3D& value);
	FilterPoint3D operator/=(int value);

	FilterROI3D operator+(const FilterROI3D& value) const;
	FilterROI3D operator-(const FilterROI3D& value) const;
	FilterROI3D operator*(const FilterROI3D& value) const;
	FilterROI3D operator/(const FilterROI3D& value) const;
};

FilterPoint3D operator+(int lhs, const FilterPoint3D& rhs);
FilterPoint3D operator-(int lhs, const FilterPoint3D& rhs);
FilterPoint3D operator*(int lhs, const FilterPoint3D& rhs);
FilterPoint3D operator/(int lhs, const FilterPoint3D& rhs);

#endif