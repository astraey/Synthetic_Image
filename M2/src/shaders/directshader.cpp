#include "directshader.h"
#include "../core/utils.h"

//DepthShader::DepthShader()
//{}

DirectShader::DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_) :
	Shader(bgColor_)

{
	this->maxDist = maxDist_;
	this->color = color_;
}

Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape *> &objList, const std::vector<PointLightSource> &lsList) const
{
	Intersection its;
	Vector3D I;
	Vector3D Distance;

	int nL = lsList.size();

	if (Utils::getClosestIntersection(r, objList, its))
	{
		for (int i = 0; i < nL; i++)
		{

			I = I.operator+(lsList[i].getIntensity(its.itsPoint));

		}
	}
	else
	{
		return bgColor;
	}

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

			for (int i = 0; i < nL; i++)
			{
				Vector3D intensity = lsList[i].getIntensity(its.itsPoint);
				Vector3D reflectance = its.shape->getMaterial().getReflectance(); // FALTA PASAR N, WI, WO DE PHONG::GETREFLECTANCE
				I = I.operator+(Utils::multiplyPerCanal(intensity, reflectance));

			}

		}

		return I;

	}
	else
	{
		return bgColor;
	}
	
}