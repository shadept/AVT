#include "Camera.h"

#ifdef _MSC_VER
#undef near
#undef far
#endif // _MSC_VER

Camera::Camera()
{
	mIsDirty = true;
}

Camera::~Camera()
{
}

void Camera::OnDraw(Renderer& renderer) const
{
	// debug draw
}

const Matrix4& Camera::GetProjection() const
{
	return mProjection;
}

Matrix4 Camera::GetProjection()
{
	return mProjection;
}

Matrix4 Camera::GetViewProjection() const
{
	if (mIsDirty || LocalTransform.IsDirty())
	{
		UpdateTransformation();
		mViewProj = mProjection * WorldTransform;
		mIsDirty = false;
	}
	return mViewProj;
}

const Matrix4& Camera::GetFrustrum() const
{
	return mProjection;
}

void Camera::SetFustrum(Real left, Real right, Real bottom, Real top, Real near, Real far)
{
	mProjection.identify();
	mProjection[0] = 2.0f / (right - left);
	mProjection[3] = -(right + left) / (right - left);
	mProjection[5] = 2.0f / (top - bottom);
	mProjection[7] = -(top + bottom) / (top - bottom);
	mProjection[10] = -2.0f / (far - near);
	mProjection[11] = -(far + near) / (far - near);
	mIsDirty = true;
}

void Camera::SetFustrum(Real fovy, Real aspectRatio, Real near, Real far)
{
	Real d = 1.0f / Math::tan(Math::radians(fovy * 0.5f));
	mProjection.identify();
	mProjection[0] = d / aspectRatio;
	mProjection[5] = d;
	mProjection[10] = (far + near) / (near - far);
	mProjection[11] = 2 * far * near / (near - far);
	mProjection[14] = -1;
	mProjection[15] = 0;
	mIsDirty = true;
}

void Camera::SetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	Vector3 v = center - eye;
	v.normalize();

	Vector3 s = v ^ up;
	s.normalize();

	Vector3 u = s ^ v;

	v = -v; // forward is -AxisZ
	Vector3 position(-s & eye, -u & eye, -v & eye);

	Quaternion rotation;
	rotation.T = Math::sqrt(1.0f + s.X + u.Y + v.Z) * 0.5f;
	if (rotation.T != 0)
	{
		Real w4_recip = 1.0f / (4.0f * rotation.T);
		rotation.X = (v.Y - u.Z) * w4_recip;
		rotation.Y = (s.Z - v.X) * w4_recip;
		rotation.Z = (u.X - s.Y) * w4_recip;
	}
	else
	{
		rotation.makeIdentity();
	}

	LocalTransform.SetPosition(position);
	LocalTransform.SetRotation(rotation);
	mIsDirty = true;
}
