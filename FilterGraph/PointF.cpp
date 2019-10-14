#include "PointF.h"


PointF::PointF() :
	x(0), y(0)
{
}

PointF::PointF(float aX, float aY) :
	x(aX), y(aY)
{
}

PointF::PointF(const PointF & aPoint) :
	x(aPoint.x), y(aPoint.y)
{
}

PointF & PointF::operator=(const PointF& arg)
{
	this->x = arg.x;
	this->y = arg.y;
	return *this;
}

bool PointF::operator==(const PointF & arg)
{
	return x == arg.x && y == arg.y;
}

PointF PointF::operator+(const PointF & value) const
{
	return PointF(x + value.x, y + value.y);
}

PointF PointF::operator+(float value) const
{
	return PointF(x + value, y + value);
}

PointF PointF::operator+=(const PointF & value)
{
	x += value.x;
	y += value.y;
	return *this;
}

PointF PointF::operator+=(float value)
{
	x += value;
	y += value;
	return *this;
}

PointF PointF::operator-(const PointF & value) const
{
	return PointF(x - value.x, y - value.y);
}

PointF PointF::operator-(float value) const
{
	return PointF(x - value, y - value);
}

PointF PointF::operator-=(const PointF & value)
{
	x -= value.x;
	y -= value.y;
	return *this;
}

PointF PointF::operator-=(float value)
{
	x -= value;
	y -= value;
	return *this;
}

PointF PointF::operator*(const PointF & value) const
{
	return PointF(x * value.x, y * value.y);
}

PointF PointF::operator*(float value) const
{
	return PointF(x * value, y * value);
}

PointF PointF::operator*=(const PointF & value)
{
	x *= value.x;
	y *= value.y;
	return *this;
}

PointF  PointF::operator*=(float value)
{
	x *= value;
	y *= value;
	return *this;
}

PointF PointF::operator/(const PointF & value) const
{
	return PointF(x / value.x, y / value.y);
}

PointF PointF::operator/(float value) const
{
	return PointF(x / value, y / value);
}

PointF PointF::operator/=(const PointF & value)
{
	x /= value.x;
	y /= value.y;
	return *this;
}

PointF PointF::operator/=(float value)
{
	x /= value;
	y /= value;
	return *this;
}

PointF operator+(float lhs, const PointF & rhs)
{
	return PointF(lhs + rhs.x, lhs + rhs.y);
}

PointF operator-(float lhs, const PointF & rhs)
{
	return PointF(lhs - rhs.x, lhs - rhs.y);
}

PointF operator*(float lhs, const PointF & rhs)
{
	return PointF(lhs * rhs.x, lhs * rhs.y);
}

PointF operator/(float lhs, const PointF & rhs)
{
	return PointF(lhs / rhs.x, lhs / rhs.y);
}


