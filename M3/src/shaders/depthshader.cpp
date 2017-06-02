#include "depthshader.h"
#include "../core/utils.h"

//DepthShader::DepthShader()
//{}

DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_) :
	Shader(bgColor_)

{
	this->maxDist = maxDist_;
	this->color = color_;
}

Vector3D DepthShader::computeColor(const Ray &r, const std::vector<Shape *> &objList, const std::vector<PointLightSource> &lsList) const
{
	Intersection its;
	Vector3D Distance;

	if (Utils::getClosestIntersection(r, objList, its))
	{
		Distance = its.itsPoint.operator-(r.o);
		double dist = Distance.length();
		
		if (dist >= maxDist)
		{

			return bgColor;

		}
		else
		{
		
			return color*(1.0 - dist/maxDist);

		}

	}
	else
	{
		return bgColor;
	}
}