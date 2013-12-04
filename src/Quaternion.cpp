#include "Quaternion.h"

#include <cassert>

const Quaternion Quaternion::Identity(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion Quaternion::fromAxisAngle(const Vector3& axis, Real angle)
{
	Quaternion q;
	angle = Math::radians(angle);
	Real s = Math::sin(angle * 0.5f);
	q.X = axis.X * s;
	q.Y = axis.Y * s;
	q.Z = axis.Z * s;
	q.T = Math::cos(angle * 0.5f);
	q.normalize();
	return q;
}

Quaternion::Quaternion()
{
	makeIdentity();
}

Quaternion::Quaternion(Real t, Real x, Real y, Real z)
{
	set(t, x, y, z);
}

Quaternion::Quaternion(const Vector3& v)
{
	set(0.0f, v.X, v.Y, v.Z);
}

Quaternion& Quaternion::makeIdentity()
{
	X = Y = Z = 0.0f;
	T = 1.0f;
	return *this;
}

void Quaternion::normalize()
{
	Real l = length2();
	if (l == 1.0f || l == 0.0f) return;
	l = 1.0f / Math::sqrt(l);
	set(T * l, X * l, Y * l, Z * l);
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(T, -X, -Y, -Z);
}

Quaternion Quaternion::inverse() const
{
	Real invl = 1.0f / length();
	return Quaternion(T * invl, -X * invl, -Y * invl, -Z * invl);
}

Quaternion Quaternion::normalized() const
{
	Quaternion q(*this);
	q.normalize();
	return q;
}

Real Quaternion::length() const
{
	return Math::sqrt(length2());
}

Real Quaternion::length2() const
{
	return T * T + X * X + Y * Y + Z * Z;
}

Quaternion& Quaternion::set(Real t, Real x, Real y, Real z)
{
	T = CLEAR_ROUND_ERROR(t);
	X = CLEAR_ROUND_ERROR(x);
	Y = CLEAR_ROUND_ERROR(y);
	Z = CLEAR_ROUND_ERROR(z);
	return *this;
}

Quaternion& Quaternion::set(const Matrix4& m)
{
	Real diag = m[0] + m[5] + m[10] + 1;
	if (diag > 0.0f)
	{
		Real scale = Math::sqrt(diag) * 2.0f;

		X = (m[9] - m[6]) / scale;
		Y = (m[2] - m[8]) / scale;
		Z = (m[4] - m[1]) / scale;
		T = 0.25f * scale;
	}
	else
	{
		if (m[0] > m[5] && m[0] > m[10])
		{
			Real scale = Math::sqrt(1.0f + m[0] - m[5] - m[10]) * 2.0f;

			T = (m[6] - m[9]) / scale;
			X = 0.25f * scale;
			Y = (m[4] + m[1]) / scale;
			Z = (m[2] + m[8]) / scale;
		}
		else if (m[5] > m[10])
		{
			Real scale = Math::sqrt(1.0f + m[5] - m[0] - m[10]) * 2.0f;

			T = (m[2] - m[8]) / scale;
			X = (m[1] + m[4]) / scale;
			Y = 0.25f * scale;
			Z = (m[6] + m[9]) / scale;
		}
		else
		{
			Real scale = Math::sqrt(1.0f + m[10] - m[0] - m[5]) * 2.0f;

			T = (m[4] - m[1]) / scale;
			X = (m[2] + m[8]) / scale;
			Y = (m[6] + m[9]) / scale;
			Z = 0.25f * scale;
		}
	}

	normalize();
	return *this;
}

Matrix4 Quaternion::toMatrix()
{
	normalize();
	assert(Math::abs(1.0f - length()) <= Math::epsilon || length2() == 0);
	return Matrix4(
			1.0f - 2.0f * Y * Y - 2.0f * Z * Z,	2.0f * X * Y - 2.0f * Z * T, 		2.0f * X * Z + 2.0f * Y * T, 		0.0f,
			2.0f * X * Y + 2.0f * Z * T,		1.0f - 2.0f * X * X - 2.0f * Z * Z, 2.0f * Y * Z - 2.0f * X * T, 		0.0f,
			2.0f * X * Z - 2.0f * Y * T, 		2.0f * Y * Z + 2.0f * X * T, 		1.0f - 2.0f * X * X - 2.0f * Y * Y, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
}

void Quaternion::toAxisAngle(Vector3& axis, Real& angle) const
{
	angle = Math::degrees(2.0f * Math::acos(T));
	Real inv = 1.0f / Math::sqrt(1.0f - T * T);
	axis.X = X * inv;
	axis.Y = Y * inv;
	axis.Z = Z * inv;
}

Quaternion Quaternion::operator +(const Quaternion& rhs) const
{
	return Quaternion(T + rhs.T, X + rhs.X, Y + rhs.Y, Z + rhs.Z);
}

Quaternion& Quaternion::operator +=(const Quaternion& rhs)
{
	set(T + rhs.T, X + rhs.X, Y + rhs.Y, Z + rhs.Z);
	return *this;
}

Quaternion Quaternion::operator *(const Quaternion& rhs) const
{
	Quaternion q(
			T * rhs.T - X * rhs.X - Y * rhs.Y - Z * rhs.Z,
			T * rhs.X + rhs.T * X + Y * rhs.Z - Z * rhs.Y,
			T * rhs.Y + rhs.T * Y + Z * rhs.X - X * rhs.Z,
			T * rhs.Z + rhs.T * Z + X * rhs.Y - Y * rhs.X);
//	q.normalize();
	return q;
}

Quaternion& Quaternion::operator *=(const Quaternion& rhs)
{
	set(
			T * rhs.T - X * rhs.X - Y * rhs.Y - Z * rhs.Z,
			T * rhs.X + rhs.T * X + Y * rhs.Z - Z * rhs.Y,
			T * rhs.Y + rhs.T * Y + Z * rhs.X - X * rhs.Z,
			T * rhs.Z + rhs.T * Z + X * rhs.Y - Y * rhs.X);
//	normalize();
	return *this;
}

Vector3 Quaternion::operator *(const Vector3& rhs) const
{
//	Vector3 vn(rhs);
//
//	Quaternion qv(vn), qr;
//	qr = *this * qv * inverse();
//
//	return Vector3(qr.X, qr.Y, qr.Z);
	Vector3 uv, uuv;
	Vector3 qvec(X, Y, Z);
	uv = qvec ^ rhs;
	uuv = qvec ^ uv;
	uv = uv * (2.0f * T);
	uuv = uuv * 2.0f;

	return rhs + uv + uuv;
}

Quaternion Quaternion::operator *(Real f) const
{
	return Quaternion(T * f, X * f, Y * f, Z * f);
}

Quaternion operator *(Real a, const Quaternion& q)
{
	return q * a;
}

std::ostream& operator <<(std::ostream& os, const Quaternion& q)
{
	os << "(" << q.T << ",\t" << q.X << ",\t" << q.Y << ",\t" << q.Z << ")";
	return os;
}
