#include "Camera.h"

#ifdef _MSC_VER
#undef near
#undef far
#endif // _MSC_VER

Camera::Camera(const std::string& name) :
	Spatial(name)
{
	mIsDirty = true;
	mWidth = mHeight = 0;
}

Camera::~Camera()
{
}

void Camera::OnDraw(Renderer& renderer) const
{
	// debug draw
}

void Camera::SetViewport(int width, int height)
{
	glViewport(0, 0, width, height);
	mWidth = width;
	mHeight = height;
}

int Camera::GetWidth() const
{
	return mWidth;
}

int Camera::GetHeight() const
{
	return mHeight;
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

void Camera::SetOtho(Real left, Real right, Real bottom, Real top, Real near, Real far)
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

void Camera::SetFustrum(Real left, Real right, Real bottom, Real top, Real near, Real far)
{
	mProjection.identify();
	mProjection[0] = (2.0f * near) / (right - left);
	mProjection[2] = (right + left) / (right - left);
	mProjection[5] = (2.0f * near) / (top - bottom);
	mProjection[6] = (top + bottom) / (top - bottom);
	mProjection[10] = -(far + near) / (far - near);
	mProjection[11] = -(2.0f * far * near) / (far - near);
	mProjection[14] = -1;
	mIsDirty = true;
}

void Camera::SetPerspective(Real fovy, Real aspectRatio, Real near, Real far)
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

//	float fW, fH;
//
//	fH = Math::tan(fovy / 360.0f * Math::PI);
//	fW = fH * aspectRatio;
//	SetFustrum(-fW, fW, -fH, fH, near, far);
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

	Matrix4 lookAt{s, u, v};
	lookAt[3] = position.X;
	lookAt[7] = position.Y;
	lookAt[11] = position.Z;

	Quaternion rotation;
	rotation = Quaternion::fromMatrix(lookAt);

	LocalTransform.SetPosition(position);
	LocalTransform.SetRotation(rotation);
	mIsDirty = true;
}
