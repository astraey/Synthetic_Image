#ifndef GLOBALSHADER
#define GLOBALSHADER

#include "shader.h"
#include "../alDir/hemisphericalsampler.h"

class GlobalShader : public Shader
{
public:
	GlobalShader();
	GlobalShader(Vector3D bgColor_,int maxDepth, int ray );

	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;

	Vector3D bgColor;
	int maxDepth;
	int ray;
	HemisphericalSampler sampler;
};
#endif // !GLOBALSHADER
