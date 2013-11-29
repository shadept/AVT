#ifndef CAMERA_H_
#define CAMERA_H_

#include "Renderer.h"
#include "Matrix4.h"
#include "Spatial.h"

class Camera: public Spatial
{
public:
	Camera();
	virtual ~Camera();

	void OnDraw(Renderer& renderer) const;

	const Matrix4& GetProjection() const;
	Matrix4 GetProjection();
	Matrix4 GetViewProjection() const;

	const Matrix4& GetFrustrum() const;
	void SetFustrum(Real left, Real right, Real bottom, Real top, Real near, Real far);
	void SetFustrum(Real fovy, Real aspectRatio, Real near, Real far);

	void SetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up = Vector3::AxisY);

private:
	Matrix4 mProjection;

	// cached values
	mutable Matrix4 mViewProj;
	mutable bool mIsDirty;
};

#endif /* CAMERA_H_ */
