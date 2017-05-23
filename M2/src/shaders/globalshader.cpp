#include "globalshader.h"
#include "../core/utils.h"

//DepthShader::DepthShader()
//{}

GlobalShader::GlobalShader(Vector3D color_, double maxDist_, Vector3D bgColor_, Vector3D at_) :
	Shader(bgColor_)

{
	this->maxDist = maxDist_;
	this->color = color_;
	this->at = at_;
	
}

Vector3D GlobalShader::computeColor(const Ray &r, const std::vector<Shape *> &objList, const std::vector<PointLightSource> &lsList) const
{


	Intersection its;
	Vector3D Distance;
	int nL = lsList.size();
	Ray R;
	Vector3D wi;
	Vector3D wo;

	Vector3D returnColor = bgColor;

	if (Utils::getClosestIntersection(r, objList, its))
	{
		for (int i = 0; i < nL; i++)
		{
			Vector3D D = lsList[i].getPosition() - its.itsPoint;
			wi = D.normalized();

			R = Ray(its.itsPoint, wi);
			R.maxT = D.length();

			//wo = (r.o - its.itsPoint).normalized();
			wo = -r.d;

			//if (dot(wo, wi) > 0)
			if (dot(its.normal, wi) > 0)
			{

				if (!Utils::hasIntersection(R, objList))
				{

					if (its.shape->getMaterial().hasSpecular())
					{

						Vector3D wr = its.normal*(dot(wo, its.normal))*2 - wo;
						Ray R2 = Ray(its.itsPoint, wr);

						return computeColor(R2, objList, lsList);

					}

					else if (its.shape->getMaterial().hasDiffuseOrGlossy()) {

						Vector3D I = lsList[i].getIntensity(its.itsPoint);
						//Vector3D I = Vector3D(1, 1, 1);

						Vector3D R = its.shape->getMaterial().getReflectance(its.normal, wo, wi);

						returnColor += Utils::multiplyPerCanal(I, R);

						//NEW CODE
						//std::cout << R << wo << wi << std::endl;
					
					}
				}

			}
		}

		return returnColor + Utils::multiplyPerCanal(this->at, its.shape->getMaterial().getDiffuseCoefficient());

	}
	else
	{
		return bgColor;
	}

}
