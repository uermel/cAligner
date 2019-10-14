#ifndef FILTERSIZE_H
#define FILTERSIZE_H

#include "FilterROI.h"

class FilterSize
{
public:
	int width;
	int height;

	FilterSize();
	FilterSize(int aWidth, int aHeight);
	FilterSize(const FilterSize& aSize);


	FilterSize& operator=(const FilterSize& arg);
	bool operator==(const FilterSize& arg);

	FilterSize operator+(int value) const;
	FilterSize operator+(const FilterSize& value) const;

	FilterSize& operator+=(int value);
	FilterSize& operator+=(const FilterSize& value);

	FilterSize operator-(int value) const;
	FilterSize operator-(const FilterSize& value) const;

	FilterSize& operator-=(int value);
	FilterSize& operator-=(const FilterSize& value);

	FilterSize operator*(int value) const;
	FilterSize operator*(const FilterSize& value) const;

	FilterSize& operator*=(int value);
	FilterSize& operator*=(const FilterSize& value);

	FilterSize operator/(int value) const;
	FilterSize operator/(const FilterSize& value) const;

	FilterSize& operator/=(int value);
	FilterSize& operator/=(const FilterSize& value);

	FilterROI operator+(const FilterROI& value) const;
	FilterROI operator-(const FilterROI& value) const;
	FilterROI operator*(const FilterROI& value) const;
	FilterROI operator/(const FilterROI& value) const;
};


FilterSize operator+(int lhs, const FilterSize& rhs);
FilterSize operator-(int lhs, const FilterSize& rhs);
FilterSize operator*(int lhs, const FilterSize& rhs);
FilterSize operator/(int lhs, const FilterSize& rhs);

#endif