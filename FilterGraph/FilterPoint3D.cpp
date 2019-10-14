#include "FilterPoint3D.h"
#include "FilterSize3D.h"
#include "FilterROI3D.h"

FilterPoint3D::FilterPoint3D() :
	x(0), y(0)
{
}

FilterPoint3D::FilterPoint3D(int aX, int aY, int aZ) :
	x(aX), y(aY), z(aZ)
{
}

FilterPoint3D::FilterPoint3D(const FilterPoint3D & aPoint) :
	x(aPoint.x), y(aPoint.y), z(aPoint.z)
{
}

FilterPoint3D & FilterPoint3D::operator=(const FilterPoint3D& arg)
{
	this->x = arg.x;
	this->y = arg.y;
	this->z = arg.z;
	return *this;
}

bool FilterPoint3D::operator==(const FilterPoint3D & arg)
{
	return x == arg.x && y == arg.y && z == arg.z;
}

FilterPoint3D FilterPoint3D::operator+(const FilterPoint3D & value) const
{
	return FilterPoint3D(x + value.x, y + value.y, z + value.z);
}

FilterPoint3D FilterPoint3D::operator+(const FilterSize3D & value) const
{
	return FilterPoint3D(x + value.width, y + value.height, z + value.depth);
}

FilterPoint3D FilterPoint3D::operator+(int value) const
{
	return FilterPoint3D(x + value, y + value, z + value);
}

FilterPoint3D FilterPoint3D::operator+=(const FilterPoint3D & value)
{
	x += value.x;
	y += value.y;
	z += value.z;
	return *this;
}

FilterPoint3D FilterPoint3D::operator+=(const FilterSize3D & value)
{
	x += value.width;
	y += value.height;
	z += value.depth;
	return *this;
}

FilterPoint3D FilterPoint3D::operator+=(int value)
{
	x += value;
	y += value;
	z += value;
	return *this;
}

FilterPoint3D FilterPoint3D::operator-(const FilterPoint3D & value) const
{
	return FilterPoint3D(x - value.x, y - value.y, z - value.z);
}

FilterPoint3D FilterPoint3D::operator-(const FilterSize3D & value) const
{
	return FilterPoint3D(x - value.width, y - value.height, z - value.depth);
}

FilterPoint3D FilterPoint3D::operator-(int value) const
{
	return FilterPoint3D(x - value, y - value, z - value);
}

FilterPoint3D FilterPoint3D::operator-=(const FilterPoint3D & value)
{
	x -= value.x;
	y -= value.y;
	z -= value.z;
	return *this;
}

FilterPoint3D FilterPoint3D::operator-=(const FilterSize3D & value)
{
	x -= value.width;
	y -= value.height;
	z -= value.depth;
	return *this;
}

FilterPoint3D FilterPoint3D::operator-=(int value)
{
	x -= value;
	y -= value;
	z -= value;
	return *this;
}

FilterPoint3D FilterPoint3D::operator*(const FilterPoint3D & value) const
{
	return FilterPoint3D(x * value.x, y * value.y, z * value.z);
}

FilterPoint3D FilterPoint3D::operator*(const FilterSize3D & value) const
{
	return FilterPoint3D(x * value.width, y * value.height, z * value.depth);
}

FilterPoint3D FilterPoint3D::operator*(int value) const
{
	return FilterPoint3D(x * value, y * value, z * value);
}

FilterPoint3D FilterPoint3D::operator*=(const FilterPoint3D & value)
{
	x *= value.x;
	y *= value.y;
	z *= value.z;
	return *this;
}

FilterPoint3D  FilterPoint3D::operator*=(const FilterSize3D & value)
{
	x *= value.width;
	y *= value.height;
	z *= value.depth;
	return *this;
}

FilterPoint3D  FilterPoint3D::operator*=(int value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

FilterPoint3D FilterPoint3D::operator/(const FilterPoint3D & value) const
{
	return FilterPoint3D(x / value.x, y / value.y, z / value.z);
}

FilterPoint3D FilterPoint3D::operator/(const FilterSize3D & value) const
{
	return FilterPoint3D(x / value.width, y / value.height, z / value.depth);
}

FilterPoint3D FilterPoint3D::operator/(int value) const
{
	return FilterPoint3D(x / value, y / value, z / value);
}

FilterPoint3D FilterPoint3D::operator/=(const FilterPoint3D & value)
{
	x /= value.x;
	y /= value.y;
	z /= value.z;
	return *this;
}

FilterPoint3D FilterPoint3D::operator/=(const FilterSize3D & value)
{
	x /= value.width;
	y /= value.height;
	z /= value.depth;
	return *this;
}

FilterPoint3D FilterPoint3D::operator/=(int value)
{
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

FilterROI3D FilterPoint3D::operator+(const FilterROI3D & value) const
{
	return FilterROI3D(x + value.x, y + value.y, z + value.z, value.width, value.height, value.depth);
}

FilterROI3D FilterPoint3D::operator-(const FilterROI3D & value) const
{
	return FilterROI3D(x - value.x, y - value.y, z - value.z, value.width, value.height, value.depth);
}

FilterROI3D FilterPoint3D::operator*(const FilterROI3D & value) const
{
	return FilterROI3D(x * value.x, y * value.y, z * value.z, value.width, value.height, value.depth);
}

FilterROI3D FilterPoint3D::operator/(const FilterROI3D & value) const
{
	return FilterROI3D(x / value.x, y / value.y, z / value.z, value.width, value.height, value.depth);
}

FilterPoint3D operator+(int lhs, const FilterPoint3D & rhs)
{
	return FilterPoint3D(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
}

FilterPoint3D operator-(int lhs, const FilterPoint3D & rhs)
{
	return FilterPoint3D(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
}

FilterPoint3D operator*(int lhs, const FilterPoint3D & rhs)
{
	return FilterPoint3D(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

FilterPoint3D operator/(int lhs, const FilterPoint3D & rhs)
{
	return FilterPoint3D(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
}


