#ifndef MATERIAL_H_
#define MATERIAL_H_

struct Color
{
	Color(float r, float b, float g);

	float R, G, B;
};

class Material
{
public:
	Material();
	virtual ~Material();

public:
	Color mAmbient;
	Color mDiffuse;
	Color mSpecular;
};

#endif /* MATERIAL_H_ */
