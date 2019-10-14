#ifndef FILTERPOINT2D_H
#define FILTERPOINT2D_H

#include "../Basics/Default.h"

//Forward declatation
class FilterSize;
class FilterROI;

class FilterPoint2D
{
public:
	int x;
	int y;

	FilterPoint2D();
	FilterPoint2D(int aX, int aY);
	FilterPoint2D(const FilterPoint2D& aPoint);

	FilterPoint2D& operator=(const FilterPoint2D& arg);
	bool operator==(const FilterPoint2D& arg);

	FilterPoint2D operator+(const FilterPoint2D& value) const;
	FilterPoint2D operator+(const FilterSize& value) const;
	FilterPoint2D operator+(int value) const;

	FilterPoint2D operator+=(const FilterPoint2D& value);
	FilterPoint2D operator+=(const FilterSize& value);
	FilterPoint2D operator+=(int value);

	FilterPoint2D operator-(const FilterPoint2D& value) const;
	FilterPoint2D operator-(const FilterSize& value) const;
	FilterPoint2D operator-(int value) const;

	FilterPoint2D operator-=(const FilterPoint2D& value);
	FilterPoint2D operator-=(const FilterSize& value);
	FilterPoint2D operator-=(int value);

	FilterPoint2D operator*(const FilterPoint2D& value) const;
	FilterPoint2D operator*(const FilterSize& value) const;
	FilterPoint2D operator*(int value) const;

	FilterPoint2D operator*=(const FilterPoint2D& value);
	FilterPoint2D operator*=(const FilterSize& value);
	FilterPoint2D operator*=(int value);

	FilterPoint2D operator/(const FilterPoint2D& value) const;
	FilterPoint2D operator/(const FilterSize& value) const;
	FilterPoint2D operator/(int value) const;

	FilterPoint2D operator/=(const FilterPoint2D& value);
	FilterPoint2D operator/=(const FilterSize& value);
	FilterPoint2D operator/=(int value);

	FilterROI operator+(const FilterROI& value) const;
	FilterROI operator-(const FilterROI& value) const;
	FilterROI operator*(const FilterROI& value) const;
	FilterROI operator/(const FilterROI& value) const;
};

FilterPoint2D operator+(int lhs, const FilterPoint2D& rhs);
FilterPoint2D operator-(int lhs, const FilterPoint2D& rhs);
FilterPoint2D operator*(int lhs, const FilterPoint2D& rhs);
FilterPoint2D operator/(int lhs, const FilterPoint2D& rhs);

#endif