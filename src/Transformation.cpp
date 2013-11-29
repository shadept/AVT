#include "Transformation.h"

#include <iostream>

Transformation::Transformation()
{
	//mRotation.set(1.0f, 0.0f, 0.0f, 0.0f);
	mScale.set(1.0f, 1.0f, 1.0f);
	//mPosition.set(0.0f, 0.0f, 0.0f);
	mIsDirty = true;
}

Transformation::Transformation(const Transformation& rhs)
{
	*this = rhs;
}

Transformation& Transformation::operator =(const Transformation& rhs)
{
	mRotation = rhs.mRotation;
	mScale = rhs.mScale;
	mPosition = rhs.mPosition;
	if (rhs.IsDirty())
	{
		mIsDirty = true;
	}
	else
	{
		mMatrix = rhs.mMatrix;
		mIsDirty = false;
	}
	return *this;
}

Transformation Transformation::operator *(const Transformation& rhs) const
{
	Transformation ret;
	ret.mPosition = mRotation * (mScale * rhs.mPosition) + mPosition;
	ret.mRotation = mRotation * rhs.mRotation;
	ret.mScale = mScale * rhs.mScale;
	return ret;
}

Matrix4 Transformation::GetMatrix() const
{
	Update();
	return mMatrix;
}

Transformation::operator Matrix4() const
{
	Update();
	return mMatrix;
}

Transformation& Transformation::Rotate(const Quaternion& rotation)
{
	mRotation *= rotation;
	mIsDirty = true;
	return *this;
}

Transformation& Transformation::Rotate(const Vector3& axis, Real angle)
{
	mRotation *= Quaternion::fromAxisAngle(axis, angle);
	mIsDirty = true;
	return *this;
}

Transformation& Transformation::Translate(const Vector3& translation)
{
	mPosition += translation;
	mIsDirty = true;
	return *this;
}

Transformation& Transformation::Scale(const Vector3& scale)
{
	mScale *= scale;
	mIsDirty = true;
	return *this;
}

Transformation& Transformation::SetRotation(const Quaternion& rotation)
{
	mRotation = rotation;
	mIsDirty = true;
	return *this;
}

Transformation& Transformation::SetRotation(const Vector3& axis, Real angle)
{
	mRotation = Quaternion::fromAxisAngle(axis, angle);
	mIsDirty = true;
	return *this;
}

Transformation& Transformation::SetScale(const Vector3& scale)
{
	mScale = scale;
	mIsDirty = true;
	return *this;
}

Transformation& Transformation::SetPosition(const Vector3& position)
{
	mPosition = position;
	mIsDirty = true;
	return *this;
}

Vector3 Transformation::Side() const
{
	Update();
	return Vector3(mMatrix[0], mMatrix[1], mMatrix[2]);
}

Vector3 Transformation::Up() const
{
	Update();
	return Vector3(mMatrix[4], mMatrix[5], mMatrix[6]);
}

Vector3 Transformation::Forward() const
{
	Update();
	return Vector3(-mMatrix[8], -mMatrix[9], -mMatrix[10]);
}

Vector3 Transformation::Position() const
{
	return mPosition;
}

Quaternion  Transformation::Orientation() const
{
	return mRotation;
}

Vector3  Transformation::Scale() const
{
	return mScale;
}

bool Transformation::IsDirty() const
{
	return mIsDirty;
}

void Transformation::Update() const
{
	if (mIsDirty)
	{
		Matrix4 translate = Matrix4::fromTranslation(mPosition);
		Matrix4 rotate = Matrix4::fromQuaternion(mRotation);
		Matrix4 scale = Matrix4::fromScale(mScale);
		mMatrix = translate * rotate * scale;
		mIsDirty = false;
	}
}
