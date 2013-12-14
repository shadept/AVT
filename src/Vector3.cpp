#include "Vector3.h"
#include <utility>

Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
Vector3 Vector3::AxisX(1.0f, 0.0f, 0.0f);
Vector3 Vector3::AxisY(0.0f, 1.0f, 0.0f);
Vector3 Vector3::AxisZ(0.0f, 0.0f, 1.0f);

Vector3 Vector3::OrthoNormalize(const Vector3& normal, const Vector3& tangent) {
	Vector3 proj = normal * (tangent & normal);
	Vector3 r = tangent - proj;
	r.normalize();
	return r;
}


Vector3::Vector3() :
		X(0), Y(0), Z(0)
{
}

Vector3::Vector3(Real x, Real y, Real z) :
		X(x), Y(y), Z(z)
{
}

Real Vector3::length() const
{
	return Math::sqrt(length2());
}

Real Vector3::length2() const
{
	return X * X + Y * Y + Z * Z;
}

void Vector3::normalize()
{
	Real l = length2();
	if (l == 1.0f || l == 0.0f)
		return;
	l = 1.0f / Math::sqrt(l);
	set(X * l, Y * l, Z * l);
}

void Vector3::set(Real x, Real y, Real z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

const Real* Vector3::get() const
{
	return &X;
}

Vector3::operator const Real*() const
{
	return &X;
}

Vector3 Vector3::operator +(const Vector3& rhs) const
{
	return Vector3(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
}

Vector3& Vector3::operator +=(const Vector3& rhs)
{
	set(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
	return *this;
}

Vector3 Vector3::operator -(const Vector3& rhs) const
{
	return Vector3(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
}

Vector3& Vector3::operator -=(const Vector3& rhs)
{
	set(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
	return *this;
}

Vector3 Vector3::operator -() const
{
	return Vector3(-X, -Y, -Z);
}

Vector3 Vector3::operator *(const Vector3& rhs) const
{
	return Vector3(X * rhs.X, Y * rhs.Y, Z * rhs.Z);
}

Vector3& Vector3::operator *=(const Vector3& rhs)
{
	set(X * rhs.X, Y * rhs.Y, Z * rhs.Z);
	return *this;
}

Vector3 Vector3::operator *(Real a) const
{
	return Vector3(a * X, a * Y, a * Z);
}

Real Vector3::operator &(const Vector3& rhs) const
{
	return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
}

Vector3 Vector3::operator ^(const Vector3& rhs) const
{
	return Vector3(Y * rhs.Z - Z * rhs.Y, Z * rhs.X - X * rhs.Z, X * rhs.Y - Y * rhs.X);
}

const Real Vector3::operator [](int index) const
{
	return (&X)[index];
}

Real Vector3::operator [](int index)
{
	return (&X)[index];
}

Vector3 operator *(Real a, const Vector3& v)
{
	return v * a;
}

bool Vector3::operator ==(const Vector3& rhs) const
{
	return (X == rhs.X && Y == rhs.Y && Z == rhs.Z);
}

bool Vector3::operator !=(const Vector3& rhs) const
{
	return (X != rhs.X || Y != rhs.Y || Z != rhs.Z);
}

std::ostream& operator <<(std::ostream& os, const Vector3& v)
{
	os << "(" << v[0] << ",\t" << v[1] << ",\t" << v[2] << ")";
	return os;
}

Vector4::Vector4() :
				X(0), Y(0), Z(0), W(0)
{
}

Vector4::Vector4(Real x, Real y, Real z, Real w) :
				X(x), Y(y), Z(z), W(w)
{
}

const Real* Vector4::get() const
{
	return &X;
}

Vector4::operator const Real*() const
{
	return &X;
}
