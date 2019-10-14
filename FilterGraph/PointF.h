#ifndef POINTF_H
#define POINTF_H

class PointF
{
public:
	float x;
	float y;

	PointF();
	PointF(float aX, float aY);
	PointF(const PointF& aPoint);

	PointF& operator=(const PointF& arg);
	bool operator==(const PointF& arg);

	PointF operator+(const PointF& value) const;
	PointF operator+(float value) const;

	PointF operator+=(const PointF& value);
	PointF operator+=(float value);

	PointF operator-(const PointF& value) const;
	PointF operator-(float value) const;

	PointF operator-=(const PointF& value);
	PointF operator-=(float value);

	PointF operator*(const PointF& value) const;
	PointF operator*(float value) const;

	PointF operator*=(const PointF& value);
	PointF operator*=(float value);

	PointF operator/(const PointF& value) const;
	PointF operator/(float value) const;

	PointF operator/=(const PointF& value);
	PointF operator/=(float value);
};

PointF operator+(float lhs, const PointF& rhs);
PointF operator-(float lhs, const PointF& rhs);
PointF operator*(float lhs, const PointF& rhs);
PointF operator/(float lhs, const PointF& rhs);

#endif