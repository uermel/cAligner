#include "FilterPoint2D.h"
#include "FilterSize.h"
#include "FilterROI.h"

FilterPoint2D::FilterPoint2D():
	x(0), y(0)
{
}

FilterPoint2D::FilterPoint2D(int aX, int aY):
	x(aX), y(aY)
{
}

FilterPoint2D::FilterPoint2D(const FilterPoint2D & aPoint) :
	x(aPoint.x), y(aPoint.y)
{
}

FilterPoint2D & FilterPoint2D::operator=(const FilterPoint2D& arg)
{
	this->x = arg.x;
	this->y = arg.y;
	return *this;
}

bool FilterPoint2D::operator==(const FilterPoint2D & arg)
{
	return x == arg.x && y == arg.y;
}

FilterPoint2D FilterPoint2D::operator+(const FilterPoint2D & value) const
{
	return FilterPoint2D(x + value.x, y + value.y);
}

FilterPoint2D FilterPoint2D::operator+(const FilterSize & value) const
{
	return FilterPoint2D(x + value.width, y + value.height);
}

FilterPoint2D FilterPoint2D::operator+(int value) const
{
	return FilterPoint2D(x + value, y + value);
}

FilterPoint2D FilterPoint2D::operator+=(const FilterPoint2D & value)
{
	x += value.x;
	y += value.y;
	return *this;
}

FilterPoint2D FilterPoint2D::operator+=(const FilterSize & value)
{
	x += value.width;
	y += value.height;
	return *this;
}

FilterPoint2D FilterPoint2D::operator+=(int value)
{
	x += value;
	y += value;
	return *this;
}

FilterPoint2D FilterPoint2D::operator-(const FilterPoint2D & value) const
{
	return FilterPoint2D(x - value.x, y - value.y);
}

FilterPoint2D FilterPoint2D::operator-(const FilterSize & value) const
{
	return FilterPoint2D(x - value.width, y - value.height);
}

FilterPoint2D FilterPoint2D::operator-(int value) const
{
	return FilterPoint2D(x - value, y - value);
}

FilterPoint2D FilterPoint2D::operator-=(const FilterPoint2D & value)
{
	x -= value.x;
	y -= value.y;
	return *this;
}

FilterPoint2D FilterPoint2D::operator-=(const FilterSize & value)
{
	x -= value.width;
	y -= value.height;
	return *this;
}

FilterPoint2D FilterPoint2D::operator-=(int value)
{
	x -= value;
	y -= value;
	return *this;
}

FilterPoint2D FilterPoint2D::operator*(const FilterPoint2D & value) const
{
	return FilterPoint2D(x * value.x, y * value.y);
}

FilterPoint2D FilterPoint2D::operator*(const FilterSize & value) const
{
	return FilterPoint2D(x * value.width, y * value.height);
}

FilterPoint2D FilterPoint2D::operator*(int value) const
{
	return FilterPoint2D(x * value, y * value);
}

FilterPoint2D FilterPoint2D::operator*=(const FilterPoint2D & value)
{
	x *= value.x;
	y *= value.y;
	return *this;
}

FilterPoint2D  FilterPoint2D::operator*=(const FilterSize & value)
{
	x *= value.width;
	y *= value.height;
	return *this;
}

FilterPoint2D  FilterPoint2D::operator*=(int value)
{
	x *= value;
	y *= value;
	return *this;
}

FilterPoint2D FilterPoint2D::operator/(const FilterPoint2D & value) const
{
	return FilterPoint2D(x / value.x, y / value.y);
}

FilterPoint2D FilterPoint2D::operator/(const FilterSize & value) const
{
	return FilterPoint2D(x / value.width, y / value.height);
}

FilterPoint2D FilterPoint2D::operator/(int value) const
{
	return FilterPoint2D(x / value, y / value);
}

FilterPoint2D FilterPoint2D::operator/=(const FilterPoint2D & value)
{
	x /= value.x;
	y /= value.y;
	return *this;
}

FilterPoint2D FilterPoint2D::operator/=(const FilterSize & value)
{
	x /= value.width;
	y /= value.height;
	return *this;
}

FilterPoint2D FilterPoint2D::operator/=(int value)
{
	x /= value;
	y /= value;
	return *this;
}

FilterROI FilterPoint2D::operator+(const FilterROI & value) const
{
	return FilterROI(x + value.x, y + value.y, value.width, value.height);
}

FilterROI FilterPoint2D::operator-(const FilterROI & value) const
{
	return FilterROI(x - value.x, y - value.y, value.width, value.height);
}

FilterROI FilterPoint2D::operator*(const FilterROI & value) const
{
	return FilterROI(x * value.x, y * value.y, value.width, value.height);
}

FilterROI FilterPoint2D::operator/(const FilterROI & value) const
{
	return FilterROI(x / value.x, y / value.y, value.width, value.height);
}

FilterPoint2D operator+(int lhs, const FilterPoint2D & rhs)
{
	return FilterPoint2D(lhs + rhs.x, lhs + rhs.y);
}

FilterPoint2D operator-(int lhs, const FilterPoint2D & rhs)
{
	return FilterPoint2D(lhs - rhs.x, lhs - rhs.y);
}

FilterPoint2D operator*(int lhs, const FilterPoint2D & rhs)
{
	return FilterPoint2D(lhs * rhs.x, lhs * rhs.y);
}

FilterPoint2D operator/(int lhs, const FilterPoint2D & rhs)
{
	return FilterPoint2D(lhs / rhs.x, lhs / rhs.y);
}


