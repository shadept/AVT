#ifndef MATRIX4_H_
#define MATRIX4_H_

#include "Math.h"
#include <iostream>

// All these matrices are row major

class Matrix4
{
public:
	static Matrix4 fromAxisAngle(const Vector3& axis, Real angle);
	static Matrix4 fromQuaternion(const Quaternion& quat);
	static Matrix4 fromScale(const Vector3& scale);
	static Matrix4 fromTranslation(const Vector3& translation);

public:
	Matrix4();
	Matrix4(const Real src[16]);
	Matrix4(Real xx, Real xy, Real xz, Real xw, Real yx, Real yy, Real yz, Real yw, Real zx, Real zy, Real zz, Real zw,
			Real wx, Real wy, Real wz, Real ww);

	// Modifiers
	Matrix4& identify();
	Matrix4& invert();
	Real determinant();
	Matrix4& transpose();

	// Setters
	void set(const Real src[16]);
	void set(Real xx, Real xy, Real xz, Real xw, Real yx, Real yy, Real yz, Real yw, Real zx, Real zy, Real zz, Real zw,
			Real wx, Real wy, Real wz, Real ww);

	// Getters
	const Real* get() const;
	operator const Real*() const;

	Matrix4 inverted() const;

	Matrix4 operator !() const;
	Matrix4 transposed() const;

	// Operators
	Matrix4 operator +(const Matrix4& rhs) const;
	Matrix4& operator +=(const Matrix4& rhs);
	Matrix4 operator *(const Matrix4& rhs) const;
	Matrix4& operator *=(const Matrix4& rhs);
	Vector3 operator *(const Vector3& rhs) const;
	Matrix4 operator *(Real a) const;
	Matrix4& operator *=(Real a);

	const Real& operator [](unsigned int index) const;
	Real& operator [](unsigned int index);
	const Real& operator ()(unsigned int row, unsigned int col) const;
	Real& operator ()(unsigned int row, unsigned int col);

private:
	Real m[16];
};

Matrix4 operator *(Real a, const Matrix4& m);
std::ostream& operator<<(std::ostream& os, const Matrix4& m);

#endif /* MATRIX4_H_ */

