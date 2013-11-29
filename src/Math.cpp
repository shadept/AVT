#include "Math.h"

#include <algorithm>
#include <cmath>
#include <limits>

Real Math::abs(Real f)
{
	return std::abs(f);
}

Real Math::pow(Real b, Real p)
{
	Real r = std::pow(b, p);
	return CLEAR_ROUND_ERROR(r);
}

Real Math::max(Real a, Real b)
{
	return std::max(a, b);
}

Real Math::min(Real a, Real b)
{
	return std::min(a, b);
}

Real Math::mod(Real f, Real b)
{
	return std::fmod(f, b);
}

Real Math::clamp(Real f, Real a, Real b)
{
	return f < a ? a : (f > b ? b : f);
}

Real Math::sqrt(Real f)
{
	return std::sqrt(f);
}

Real Math::radians(Real d)
{
	return d * PI / 180.0f;
}

Real Math::degrees(Real r)
{
	return r * 180.0f / PI;
}

Real Math::cos(Real f)
{
	Real r = std::cos(f);
	return CLEAR_ROUND_ERROR(r);
}

Real Math::sin(Real f)
{
	Real r = std::sin(f);
	return CLEAR_ROUND_ERROR(r);
}

Real Math::tan(Real f)
{
	Real r = std::tan(f);
	return CLEAR_ROUND_ERROR(r);
}

Real Math::acos(Real f)
{
	Real r = std::acos(f);
	return CLEAR_ROUND_ERROR(r);
}

Real Math::asin(Real f)
{
	Real r = std::asin(f);
	return CLEAR_ROUND_ERROR(r);
}

Real Math::lerp(Real r1, Real r2, Real a)
{
	return (r1 + a * (r2 - r1));
}

Vector3 Math::lerp(const Vector3& v1, const Vector3& v2, Real a)
{
	//return (v1 + a * (v2 - v1));
	return v1 * (1.0f - a) + v2 * a;
}

Quaternion Math::lerp(const Quaternion& q1, const Quaternion& q2, Real a)
{
	return q1 * (1.0f - a) + q2 * a;
}

Quaternion Math::slerp(const Quaternion& q1, const Quaternion& q2, Real a)
{
	Real angle = Math::acos(q1.T * q2.T + q1.X * q2.X + q1.Y * q2.Y + q1.Z * q2.Z);
	Real k0 = Math::sin((1.0f - a) * angle) / Math::sin(angle);
	Real k1 = Math::sin(a * angle) / Math::sin(angle);
	Quaternion q = q1 * k0 + q2 * k1;
	q.normalize();
	return q;
}
