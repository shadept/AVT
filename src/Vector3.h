#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "Math.h"
#include <iostream>

class Vector3
{
public:
	Vector3 OrthoNormalize(const Vector3& normal, const Vector3& tangent);

public:
	Vector3();
	Vector3(Real x, Real y, Real z);

	Real length() const;
	Real length2() const;

	void normalize();

	void set(Real x, Real y, Real z);

	const Real* get() const;
	operator const Real*() const;

	Vector3 operator +(const Vector3& rhs) const;
	Vector3& operator +=(const Vector3& rhs);
	Vector3 operator -(const Vector3& rhs) const;
	Vector3& operator -=(const Vector3& rhs);
	Vector3 operator -() const;

	// term by term multiplication. this is neither the dot nor cross product
	// useful for combining scale vectors
	Vector3 operator *(const Vector3& rhs) const;
	Vector3& operator *=(const Vector3& rhs);

	Vector3 operator *(Real a) const;

	// dot product
	Real operator &(const Vector3& rhs) const;

	// cross product
	Vector3 operator ^(const Vector3& rhs) const;

	const Real operator [](unsigned int index) const;
	Real operator [](unsigned int index);

	bool operator==(const Vector3& rhs) const;
	bool operator!=(const Vector3& rhs) const;

public:
	Real X, Y, Z;

	static Vector3 Zero, One;
	static Vector3 AxisX, AxisY, AxisZ;
};

Vector3 operator *(Real a, const Vector3& v);
std::ostream& operator<<(std::ostream& os, const Vector3& v);

#endif /* VECTOR3_H_ */
