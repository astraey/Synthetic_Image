#ifndef GLOBALSHADER
#define GLOBALSHADER

#include "shader.h"
#include "../sampler/hemisphericalsampler.h"

class GlobalShader : public Shader
{
public:
	GlobalShader();
	GlobalShader(Vector3D bgColor_, int rays, int bounces);

	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;

	Vector3D bgColor;
	int rays;
	int bounces;
	HemisphericalSampler sampler;
};
#endif // !GLOBALSHADER
