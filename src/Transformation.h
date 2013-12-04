#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

#include "Math.h"

class Transformation
{
public:
	Transformation();
	Transformation(const Transformation& rhs);
	Transformation& operator =(const Transformation& rhs);

	Transformation operator*(const Transformation& rhs) const;
	Vector3 operator*(const Vector3& rhs) const;

	Matrix4 GetMatrix() const;
	operator Matrix4() const;

	Transformation Inverse() const;

	Transformation& Rotate(const Quaternion& rotation);
	Transformation& Rotate(const Vector3& axis, Real angle);
	Transformation& Translate(const Vector3& translation);
	Transformation& Scale(const Vector3& scale);

	Transformation& SetRotation(const Quaternion& rotation);
	Transformation& SetRotation(const Vector3& axis, Real angle);
	Transformation& SetScale(const Vector3& scale);
	Transformation& SetPosition(const Vector3& position);

	Vector3 Side() const;
	Vector3 Up() const;
	Vector3 Forward() const;

	Vector3 Position() const;
	Quaternion Orientation() const;
	Vector3 Scale() const;

	// For internal use only
	bool IsDirty() const;
	void Update() const;

private:
	mutable bool mIsDirty;
	mutable Matrix4 mMatrix;

	Quaternion mRotation;
	Vector3 mScale;
	Vector3 mPosition;
};

#endif /* TRANSFORMATION_H_ */
