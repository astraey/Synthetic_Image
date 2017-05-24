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
	Vector3D Distance;
	int nL = lsList.size();
	Ray R;
	Vector3D wi;
	Vector3D wo = -r.d;

	Vector3D returnColor = bgColor;

	if (Utils::getClosestIntersection(r, objList, its))
	{

		//Phong
		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {

			for (int i = 0; i < nL; i++)
			{

				Vector3D D = lsList[i].getPosition() - its.itsPoint;
				wi = D.normalized();

				R = Ray(its.itsPoint, wi);
				R.maxT = D.length();

				if (!Utils::hasIntersection(R, objList))
				{

					if (dot(its.normal, wi) > 0)
					{


						Vector3D I = lsList[i].getIntensity(its.itsPoint);

						Vector3D R = its.shape->getMaterial().getReflectance(its.normal, wo, wi);

						returnColor += Utils::multiplyPerCanal(I, R);


					}
				}
			}
			return returnColor;

		}

		//MIRROR
		else if (its.shape->getMaterial().hasSpecular())
		{

			Vector3D wr = its.normal*(dot(wo, its.normal)) * 2 - wo;
			Ray R2 = Ray(its.itsPoint, wr);
			return computeColor(R2, objList, lsList);

		}

	}

	else
	{
		return bgColor;
	}
}
