#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Math.h"

class Quaternion
{
public:
	static Quaternion fromAxisAngle(const Vector3& axis, Real angle);
	static Quaternion fromMatrix(const Matrix4& mat);

public:
	Quaternion();
	Quaternion(Real t, Real x, Real y, Real z);
	Quaternion(const Vector3& v);

	Quaternion& makeIdentity();
	void normalize();

	Quaternion conjugate() const;
	Quaternion inverse() const;
	Quaternion normalized() const;

	Real length() const;
	Real length2() const;

	//
	Quaternion& set(Real t, Real x, Real y, Real z);
	Quaternion& set(const Matrix4& m);

	Matrix4 toMatrix();
	void toAxisAngle(Vector3& axis, Real& angle) const;

	//
	Quaternion operator+(const Quaternion& rhs) const;
	Quaternion& operator+=(const Quaternion& rhs);
	Quaternion operator*(const Quaternion& rhs) const;
	Quaternion& operator*=(const Quaternion& rhs);
	Vector3 operator*(const Vector3& rhs) const;
	Quaternion operator*(Real f) const; // only useful for lerping

public:
	Real T, X, Y, Z;

	static const Quaternion Identity;
};

Quaternion operator *(Real a, const Quaternion& q);
std::ostream& operator<<(std::ostream& os, const Quaternion& q);

#endif /* QUATERNION_H_ */
