#ifndef MATH_H_
#define MATH_H_

typedef float Real;
class Vector3;
class Vector4;
class Matrix3;
class Matrix4;
class Quaternion;

#define CLEAR_ROUND_ERROR(X) (Math::abs(X) >= Math::epsilon ? (X) : 0)

namespace Math
{

const Real PI = 3.141592653589793238462643383279502884197f;
const Real epsilon = 1.0e-5f;

Real abs(Real f);
Real pow(Real b, Real p);

Real max(Real a, Real b);
Real min(Real a, Real b);

Real mod(Real f, Real b);
Real clamp(Real f, Real a, Real b);

Real sqrt(Real f);

Real radians(Real d);
Real degrees(Real r);

Real cos(Real f);
Real sin(Real f);
Real tan(Real f);

Real acos(Real f);
Real asin(Real f);

Real lerp(Real r1, Real r2, Real a);
Vector3 lerp(const Vector3& v1, const Vector3& v2, Real a);
Quaternion lerp(const Quaternion& q1, const Quaternion& q2, Real a);

Quaternion slerp(const Quaternion& q1, const Quaternion& q2, Real a);

} /* namespace Math */

#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"

#endif /* MATH_H_ */
