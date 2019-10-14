#include "FilterSize.h"

FilterSize::FilterSize() :
	width(0), height(0)
{
}

FilterSize::FilterSize(int aWidth, int aHeight) :
	width(aWidth), height(aHeight)
{
}

FilterSize::FilterSize(const FilterSize & aPoint) :
	width(aPoint.width), height(aPoint.height)
{
}

FilterSize & FilterSize::operator=(const FilterSize& arg)
{
	this->width = arg.width;
	this->height = arg.height;
	return *this;
}

bool FilterSize::operator==(const FilterSize & arg)
{
	return width == arg.width && height == arg.height;
}

FilterSize FilterSize::operator+(int value) const
{
	return FilterSize(width + value, height + value);
}

FilterSize FilterSize::operator+(const FilterSize & value) const
{
	return FilterSize(width + value.width, height + value.height);
}

FilterSize & FilterSize::operator+=(int value)
{
	width += value;
	height += value;
	return *this;
}

FilterSize & FilterSize::operator+=(const FilterSize & value)
{
	width += value.width;
	height += value.height;
	return *this;
}

FilterSize FilterSize::operator-(int value) const
{
	return FilterSize(width - value, height - value);
}

FilterSize FilterSize::operator-(const FilterSize & value) const
{
	return FilterSize(width - value.width, height - value.height);
}

FilterSize & FilterSize::operator-=(int value)
{
	width -= value;
	height -= value;
	return *this;
}

FilterSize & FilterSize::operator-=(const FilterSize & value)
{
	width -= value.width;
	height -= value.height;
	return *this;
}

FilterSize FilterSize::operator*(int value) const
{
	return FilterSize(width * value, height * value);
}

FilterSize FilterSize::operator*(const FilterSize & value) const
{
	return FilterSize(width * value.width, height * value.height);
}

FilterSize & FilterSize::operator*=(int value)
{
	width *= value;
	height *= value;
	return *this;
}

FilterSize & FilterSize::operator*=(const FilterSize & value)
{
	width *= value.width;
	height *= value.height;
	return *this;
}

FilterSize FilterSize::operator/(int value) const
{
	return FilterSize(width / value, height / value);
}

FilterSize FilterSize::operator/(const FilterSize & value) const
{
	return FilterSize(width / value.width, height / value.height);
}

FilterSize & FilterSize::operator/=(int value)
{
	width /= value;
	height /= value;
	return *this;
}

FilterSize & FilterSize::operator/=(const FilterSize & value)
{
	width /= value.width;
	height /= value.height;
	return *this;
}

FilterROI FilterSize::operator+(const FilterROI & value) const
{
	return FilterROI(value.x, value.y, width + value.width, height + value.height);
}

FilterROI FilterSize::operator-(const FilterROI & value) const
{
	return FilterROI(value.x, value.y, width - value.width, height - value.height);
}

FilterROI FilterSize::operator*(const FilterROI & value) const
{
	return FilterROI(value.x, value.y, width * value.width, height * value.height);
}

FilterROI FilterSize::operator/(const FilterROI & value) const
{
	return FilterROI(value.x, value.y, width / value.width, height / value.height);
}

FilterSize operator+(int lhs, const FilterSize & rhs)
{
	return FilterSize(lhs + rhs.width, lhs + rhs.height);
}

FilterSize operator-(int lhs, const FilterSize & rhs)
{
	return FilterSize(lhs - rhs.width, lhs - rhs.height);
}

FilterSize operator*(int lhs, const FilterSize & rhs)
{
	return FilterSize(lhs * rhs.width, lhs * rhs.height);
}

FilterSize operator/(int lhs, const FilterSize & rhs)
{
	return FilterSize(lhs / rhs.width, lhs / rhs.height);
}

