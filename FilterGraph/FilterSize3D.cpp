#include "FilterSize3D.h"

FilterSize3D::FilterSize3D() :
	width(0), height(0)
{
}

FilterSize3D::FilterSize3D(int aWidth, int aHeight, int aDepth) :
	width(aWidth), height(aHeight), depth(aDepth)
{
}

FilterSize3D::FilterSize3D(const FilterSize3D & aPoint) :
	width(aPoint.width), height(aPoint.height), depth(aPoint.depth)
{
}

FilterSize3D & FilterSize3D::operator=(const FilterSize3D& arg)
{
	this->width = arg.width;
	this->height = arg.height;
	this->depth = arg.depth;
	return *this;
}

bool FilterSize3D::operator==(const FilterSize3D & arg)
{
	return width == arg.width && height == arg.height && depth == arg.depth;
}

FilterSize3D FilterSize3D::operator+(int value) const
{
	return FilterSize3D(width + value, height + value, depth + value);
}

FilterSize3D FilterSize3D::operator+(const FilterSize3D & value) const
{
	return FilterSize3D(width + value.width, height + value.height, depth + value.depth);
}

FilterSize3D & FilterSize3D::operator+=(int value)
{
	width += value;
	height += value;
	depth += value;
	return *this;
}

FilterSize3D & FilterSize3D::operator+=(const FilterSize3D & value)
{
	width += value.width;
	height += value.height;
	depth += value.depth;
	return *this;
}

FilterSize3D FilterSize3D::operator-(int value) const
{
	return FilterSize3D(width - value, height - value, depth - value);
}

FilterSize3D FilterSize3D::operator-(const FilterSize3D & value) const
{
	return FilterSize3D(width - value.width, height - value.height, depth - value.depth);
}

FilterSize3D & FilterSize3D::operator-=(int value)
{
	width -= value;
	height -= value;
	depth -= value;
	return *this;
}

FilterSize3D & FilterSize3D::operator-=(const FilterSize3D & value)
{
	width -= value.width;
	height -= value.height;
	depth -= value.height;
	return *this;
}

FilterSize3D FilterSize3D::operator*(int value) const
{
	return FilterSize3D(width * value, height * value, depth * value);
}

FilterSize3D FilterSize3D::operator*(const FilterSize3D & value) const
{
	return FilterSize3D(width * value.width, height * value.height, depth * value.depth);
}

FilterSize3D & FilterSize3D::operator*=(int value)
{
	width *= value;
	height *= value;
	depth *= value;
	return *this;
}

FilterSize3D & FilterSize3D::operator*=(const FilterSize3D & value)
{
	width *= value.width;
	height *= value.height;
	depth *= value.depth;
	return *this;
}

FilterSize3D FilterSize3D::operator/(int value) const
{
	return FilterSize3D(width / value, height / value, depth / value);
}

FilterSize3D FilterSize3D::operator/(const FilterSize3D & value) const
{
	return FilterSize3D(width / value.width, height / value.height, depth / value.depth);
}

FilterSize3D & FilterSize3D::operator/=(int value)
{
	width /= value;
	height /= value;
	depth /= value;
	return *this;
}

FilterSize3D & FilterSize3D::operator/=(const FilterSize3D & value)
{
	width /= value.width;
	height /= value.height;
	depth /= value.depth;
	return *this;
}

FilterROI3D FilterSize3D::operator+(const FilterROI3D & value) const
{
	return FilterROI3D(value.x, value.y, value.z, width + value.width, height + value.height, depth + value.depth);
}

FilterROI3D FilterSize3D::operator-(const FilterROI3D & value) const
{
	return FilterROI3D(value.x, value.y, value.z, width - value.width, height - value.height, depth - value.depth);
}

FilterROI3D FilterSize3D::operator*(const FilterROI3D & value) const
{
	return FilterROI3D(value.x, value.y, value.z, width * value.width, height * value.height, depth * value.depth);
}

FilterROI3D FilterSize3D::operator/(const FilterROI3D & value) const
{
	return FilterROI3D(value.x, value.y, value.z, width / value.width, height / value.height, depth / value.depth);
}

FilterSize3D operator+(int lhs, const FilterSize3D & rhs)
{
	return FilterSize3D(lhs + rhs.width, lhs + rhs.height, lhs + rhs.depth);
}

FilterSize3D operator-(int lhs, const FilterSize3D & rhs)
{
	return FilterSize3D(lhs - rhs.width, lhs - rhs.height, lhs - rhs.depth);
}

FilterSize3D operator*(int lhs, const FilterSize3D & rhs)
{
	return FilterSize3D(lhs * rhs.width, lhs * rhs.height, lhs * rhs.depth);
}

FilterSize3D operator/(int lhs, const FilterSize3D & rhs)
{
	return FilterSize3D(lhs / rhs.width, lhs / rhs.height, lhs / rhs.depth);
}

