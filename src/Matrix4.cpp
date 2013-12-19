#include "Matrix4.h"



Matrix3::Matrix3()
{
	m[0] = m[4] = m[8] = (Real) 1.0f;
	m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = (Real) 0.0f;
}

Matrix3::Matrix3(const Matrix4& src)
{
	m[0] = src[0];
	m[1] = src[1];
	m[2] = src[2];
	m[3] = src[4];
	m[4] = src[5];
	m[5] = src[6];
	m[6] = src[8];
	m[7] = src[9];
	m[8] = src[10];
}

Vector3 Matrix3::operator *(const Vector3& v) const
{
	return Vector3(
			m[0] * v.X + m[1] * v.Y + m[2] * v.Z,
			m[3] * v.X + m[4] * v.Y + m[5] * v.Z,
			m[6] * v.X + m[7] * v.Y + m[8] * v.Z);
}

Matrix4 Matrix4::fromAxisAngle(const Vector3& axis, Real angle)
{
	Real c = Math::cos(Math::radians(angle));
	Real s = Math::sin(Math::radians(angle));
	Real x = axis.X;
	Real y = axis.Y;
	Real z = axis.Z;
	Real xx = x * x;
	Real xy = x * y;
	Real xz = x * z;
	Real yy = y * y;
	Real yz = y * z;
	Real zz = z * z;

	Matrix4 m;
	m[0] = xx * (1 - c) + c;
	m[1] = xy * (1 - c) - z * s;
	m[2] = xz * (1 - c) + y * s;
	m[3] = 0;
	m[4] = xy * (1 - c) + z * s;
	m[5] = yy * (1 - c) + c;
	m[6] = yz * (1 - c) - x * s;
	m[7] = 0;
	m[8] = xz * (1 - c) - y * s;
	m[9] = yz * (1 - c) + x * s;
	m[10] = zz * (1 - c) + c;
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;

	return m;
}

Matrix4 Matrix4::fromQuaternion(const Quaternion& quat)
{
	Quaternion q (quat);
	return q.toMatrix();
}

Matrix4 Matrix4::fromScale(const Vector3& scale)
{
	Matrix4 m;
	m[0] = scale.X;
	m[5] = scale.Y;
	m[10] = scale.Z;
	return m;
}

Matrix4 Matrix4::fromTranslation(const Vector3& translation)
{
	Matrix4 m;
	m[3] = translation.X;
	m[7] = translation.Y;
	m[11] = translation.Z;
	return m;
}

Matrix4::Matrix4()
{
	identify();
}

Matrix4::Matrix4(const Real src[16])
{
	set(src);
}

Matrix4::Matrix4(const Vector3& row0, const Vector3& row1, const Vector3& row2)
{
	set(row0.X, row0.Y, row0.Z, 0.0f, row1.X, row1.Y, row1.Z, 0.0f, row2.X, row2.Y, row2.Z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4::Matrix4(Real xx, Real xy, Real xz, Real xw, Real yx, Real yy, Real yz, Real yw, Real zx, Real zy, Real zz,
		Real zw, Real wx, Real wy, Real wz, Real ww)
{
	set(xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw, wx, wy, wz, ww);
}

Matrix4& Matrix4::identify()
{
	m[0] = m[5] = m[10] = m[15] = (Real) 1.0f;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = (Real) 0.0f;
	return *this;
}

Real getCofactor(Real m0, Real m1, Real m2, Real m3, Real m4, Real m5, Real m6, Real m7, Real m8)
{
	return m0 * (m4 * m8 - m5 * m7) -
		   m1 * (m3 * m8 - m5 * m6) +
		   m2 * (m3 * m7 - m4 * m6);
}

Real Matrix4::determinant()
{
    return m[0] * getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]) -
           m[1] * getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]) +
           m[2] * getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]) -
           m[3] * getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);
}

Matrix4& Matrix4::invert()
{
	// get cofactors of minor matrices
	float cofactor0 = getCofactor(m[5], m[6], m[7], m[9], m[10], m[11], m[13], m[14], m[15]);
	float cofactor1 = getCofactor(m[4], m[6], m[7], m[8], m[10], m[11], m[12], m[14], m[15]);
	float cofactor2 = getCofactor(m[4], m[5], m[7], m[8], m[9], m[11], m[12], m[13], m[15]);
	float cofactor3 = getCofactor(m[4], m[5], m[6], m[8], m[9], m[10], m[12], m[13], m[14]);

	// get determinant
	float determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
	if (Math::abs(determinant) <= Math::epsilon)
	{
		identify();
		return *this;
	}

	// get rest of cofactors for adj(M)
	float cofactor4 = getCofactor(m[1], m[2], m[3], m[9], m[10], m[11], m[13], m[14], m[15]);
	float cofactor5 = getCofactor(m[0], m[2], m[3], m[8], m[10], m[11], m[12], m[14], m[15]);
	float cofactor6 = getCofactor(m[0], m[1], m[3], m[8], m[9], m[11], m[12], m[13], m[15]);
	float cofactor7 = getCofactor(m[0], m[1], m[2], m[8], m[9], m[10], m[12], m[13], m[14]);

	float cofactor8 = getCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[13], m[14], m[15]);
	float cofactor9 = getCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[12], m[14], m[15]);
	float cofactor10 = getCofactor(m[0], m[1], m[3], m[4], m[5], m[7], m[12], m[13], m[15]);
	float cofactor11 = getCofactor(m[0], m[1], m[2], m[4], m[5], m[6], m[12], m[13], m[14]);

	float cofactor12 = getCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[9], m[10], m[11]);
	float cofactor13 = getCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[8], m[10], m[11]);
	float cofactor14 = getCofactor(m[0], m[1], m[3], m[4], m[5], m[7], m[8], m[9], m[11]);
	float cofactor15 = getCofactor(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]);

	// build inverse matrix = adj(M) / det(M)
	// adjugate of M is the transpose of the cofactor matrix of M
	float invDeterminant = 1.0f / determinant;
	m[0] = invDeterminant * cofactor0;
	m[1] = -invDeterminant * cofactor4;
	m[2] = invDeterminant * cofactor8;
	m[3] = -invDeterminant * cofactor12;

	m[4] = -invDeterminant * cofactor1;
	m[5] = invDeterminant * cofactor5;
	m[6] = -invDeterminant * cofactor9;
	m[7] = invDeterminant * cofactor13;

	m[8] = invDeterminant * cofactor2;
	m[9] = -invDeterminant * cofactor6;
	m[10] = invDeterminant * cofactor10;
	m[11] = -invDeterminant * cofactor14;

	m[12] = -invDeterminant * cofactor3;
	m[13] = invDeterminant * cofactor7;
	m[14] = -invDeterminant * cofactor11;
	m[15] = invDeterminant * cofactor15;
	return *this;
}

Matrix4& Matrix4::transpose()
{
	std::swap(m[1], m[4]);
	std::swap(m[2], m[8]);
	std::swap(m[3], m[12]);
	std::swap(m[6], m[9]);
	std::swap(m[7], m[13]);
	std::swap(m[11], m[14]);
	return *this;
}

void Matrix4::set(const Real src[16])
{
	m[0] = src[0];
	m[1] = src[1];
	m[2] = src[2];
	m[3] = src[3];
	m[4] = src[4];
	m[5] = src[5];
	m[6] = src[6];
	m[7] = src[7];
	m[8] = src[8];
	m[9] = src[9];
	m[10] = src[10];
	m[11] = src[11];
	m[12] = src[12];
	m[13] = src[13];
	m[14] = src[14];
	m[15] = src[15];
}

void Matrix4::set(Real xx, Real xy, Real xz, Real xw, Real yx, Real yy, Real yz, Real yw, Real zx, Real zy, Real zz,
		Real zw, Real wx, Real wy, Real wz, Real ww)
{
	m[0] = CLEAR_ROUND_ERROR(xx);
	m[1] = CLEAR_ROUND_ERROR(xy);
	m[2] = CLEAR_ROUND_ERROR(xz);
	m[3] = CLEAR_ROUND_ERROR(xw);
	m[4] = CLEAR_ROUND_ERROR(yx);
	m[5] = CLEAR_ROUND_ERROR(yy);
	m[6] = CLEAR_ROUND_ERROR(yz);
	m[7] = CLEAR_ROUND_ERROR(yw);
	m[8] = CLEAR_ROUND_ERROR(zx);
	m[9] = CLEAR_ROUND_ERROR(zy);
	m[10] = CLEAR_ROUND_ERROR(zz);
	m[11] = CLEAR_ROUND_ERROR(zw);
	m[12] = CLEAR_ROUND_ERROR(wx);
	m[13] = CLEAR_ROUND_ERROR(wy);
	m[14] = CLEAR_ROUND_ERROR(wz);
	m[15] = CLEAR_ROUND_ERROR(ww);
}

const Real* Matrix4::get() const
{
	return m;
}

Matrix4::operator const Real*() const
{
	return m;
}

Matrix4 Matrix4::inverted() const
{
	Matrix4 r(*this);
	r.invert();
	return r;
}

Matrix4 Matrix4::operator !() const
{
	return this->transposed();
}

Matrix4 Matrix4::transposed() const
{
	Matrix4 r(*this);
	std::swap(r[1], r[4]);
	std::swap(r[2], r[8]);
	std::swap(r[3], r[12]);
	std::swap(r[6], r[9]);
	std::swap(r[7], r[13]);
	std::swap(r[11], r[14]);
	return r;
}

Matrix4 Matrix4::operator +(const Matrix4& rhs) const
{
	return Matrix4(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3], m[4] + rhs[4], m[5] + rhs[5],
			m[6] + rhs[6], m[7] + rhs[7], m[8] + rhs[8], m[9] + rhs[9], m[10] + rhs[10], m[11] + rhs[11],
			m[12] + rhs[12], m[13] + rhs[13], m[14] + rhs[14], m[15] + rhs[15]);
}

Matrix4& Matrix4::operator +=(const Matrix4& rhs)
{
	m[0] += rhs[0];
	m[1] += rhs[1];
	m[2] += rhs[2];
	m[3] += rhs[3];
	m[4] += rhs[4];
	m[5] += rhs[5];
	m[6] += rhs[6];
	m[7] += rhs[7];
	m[8] += rhs[8];
	m[9] += rhs[9];
	m[10] += rhs[10];
	m[11] += rhs[11];
	m[12] += rhs[12];
	m[13] += rhs[13];
	m[14] += rhs[14];
	m[15] += rhs[15];
	return *this;
}

Matrix4 Matrix4::operator *(const Matrix4& rhs) const
{
	return Matrix4(m[0] * rhs[0] + m[1] * rhs[4] + m[2] * rhs[8] + m[3] * rhs[12],
			m[0] * rhs[1] + m[1] * rhs[5] + m[2] * rhs[9] + m[3] * rhs[13],
			m[0] * rhs[2] + m[1] * rhs[6] + m[2] * rhs[10] + m[3] * rhs[14],
			m[0] * rhs[3] + m[1] * rhs[7] + m[2] * rhs[11] + m[3] * rhs[15],
			m[4] * rhs[0] + m[5] * rhs[4] + m[6] * rhs[8] + m[7] * rhs[12],
			m[4] * rhs[1] + m[5] * rhs[5] + m[6] * rhs[9] + m[7] * rhs[13],
			m[4] * rhs[2] + m[5] * rhs[6] + m[6] * rhs[10] + m[7] * rhs[14],
			m[4] * rhs[3] + m[5] * rhs[7] + m[6] * rhs[11] + m[7] * rhs[15],
			m[8] * rhs[0] + m[9] * rhs[4] + m[10] * rhs[8] + m[11] * rhs[12],
			m[8] * rhs[1] + m[9] * rhs[5] + m[10] * rhs[9] + m[11] * rhs[13],
			m[8] * rhs[2] + m[9] * rhs[6] + m[10] * rhs[10] + m[11] * rhs[14],
			m[8] * rhs[3] + m[9] * rhs[7] + m[10] * rhs[11] + m[11] * rhs[15],
			m[12] * rhs[0] + m[13] * rhs[4] + m[14] * rhs[8] + m[15] * rhs[12],
			m[12] * rhs[1] + m[13] * rhs[5] + m[14] * rhs[9] + m[15] * rhs[13],
			m[12] * rhs[2] + m[13] * rhs[6] + m[14] * rhs[10] + m[15] * rhs[14],
			m[12] * rhs[3] + m[13] * rhs[7] + m[14] * rhs[11] + m[15] * rhs[15]);
}

Matrix4& Matrix4::operator *=(const Matrix4& rhs)
{
	set(m[0] * rhs[0] + m[1] * rhs[4] + m[2] * rhs[8] + m[3] * rhs[12],
			m[0] * rhs[1] + m[1] * rhs[5] + m[2] * rhs[9] + m[3] * rhs[13],
			m[0] * rhs[2] + m[1] * rhs[6] + m[2] * rhs[10] + m[3] * rhs[14],
			m[0] * rhs[3] + m[1] * rhs[7] + m[2] * rhs[11] + m[3] * rhs[15],
			m[4] * rhs[0] + m[5] * rhs[4] + m[6] * rhs[8] + m[7] * rhs[12],
			m[4] * rhs[1] + m[5] * rhs[5] + m[6] * rhs[9] + m[7] * rhs[13],
			m[4] * rhs[2] + m[5] * rhs[6] + m[6] * rhs[10] + m[7] * rhs[14],
			m[4] * rhs[3] + m[5] * rhs[7] + m[6] * rhs[11] + m[7] * rhs[15],
			m[8] * rhs[0] + m[9] * rhs[4] + m[10] * rhs[8] + m[11] * rhs[12],
			m[8] * rhs[1] + m[9] * rhs[5] + m[10] * rhs[9] + m[11] * rhs[13],
			m[8] * rhs[2] + m[9] * rhs[6] + m[10] * rhs[10] + m[11] * rhs[14],
			m[8] * rhs[3] + m[9] * rhs[7] + m[10] * rhs[11] + m[11] * rhs[15],
			m[12] * rhs[0] + m[13] * rhs[4] + m[14] * rhs[8] + m[15] * rhs[12],
			m[12] * rhs[1] + m[13] * rhs[5] + m[14] * rhs[9] + m[15] * rhs[13],
			m[12] * rhs[2] + m[13] * rhs[6] + m[14] * rhs[10] + m[15] * rhs[14],
			m[12] * rhs[3] + m[13] * rhs[7] + m[14] * rhs[11] + m[15] * rhs[15]);
	return *this;
}

Vector3 Matrix4::operator *(const Vector3& rhs) const
{
	return Vector3(
			m[0] * rhs.X + m[1] * rhs.Y + m[2] * rhs.Z,
			m[4] * rhs.X + m[5] * rhs.Y + m[6] * rhs.Z,
			m[8] * rhs.X + m[9] * rhs.Y + m[10] * rhs.Z);
}

Matrix4 Matrix4::operator *(Real a) const
{
	return Matrix4(m[0] * a, m[1] * a, m[2] * a, m[3] * a, m[4] * a, m[5] * a, m[6] * a, m[7] * a, m[8] * a, m[9] * a,
			m[10] * a, m[11] * a, m[12] * a, m[13] * a, m[14] * a, m[15] * a);
}

Matrix4& Matrix4::operator *=(Real a)
{
	set(m[0] * a, m[1] * a, m[2] * a, m[3] * a, m[4] * a, m[5] * a, m[6] * a, m[7] * a, m[8] * a, m[9] * a, m[10] * a,
			m[11] * a, m[12] * a, m[13] * a, m[14] * a, m[15] * a);
	return *this;
}

const Real& Matrix4::operator [](int index) const
{
	return m[index];
}

Real& Matrix4::operator [](int index)
{
	return m[index];
}

Matrix4 operator *(Real a, const Matrix4& m)
{
	return m * a;
}

const Real& Matrix4::operator ()(int row, int col) const
{
	return m[row * 4 + col];
}

Real& Matrix4::operator ()(int row, int col)
{
	return m[row * 4 + col];
}

std::ostream& operator <<(std::ostream& os, const Matrix4& m)
{
	os << "(" << m[0] << ",\t" << m[1] << ",\t" << m[2] << ",\t" << m[3] << ")\n" << "(" << m[4] << ",\t" << m[5]
			<< ",\t" << m[6] << ",\t" << m[7] << ")\n" << "(" << m[8] << ",\t" << m[9] << ",\t" << m[10] << ",\t"
			<< m[11] << ")\n" << "(" << m[12] << ",\t" << m[13] << ",\t" << m[14] << ",\t" << m[15] << ")";
	return os;
}
