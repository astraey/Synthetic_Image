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
	Vector3D wo;

	Vector3D returnColor = bgColor;

	if (Utils::getClosestIntersection(r, objList, its))
	{
		for (int i = 0; i < nL; i++)
		{
			Vector3D D = lsList[i].getPosition() - its.itsPoint;
			wi = D.normalized();

			R = Ray(its.itsPoint, D.normalized());
			R.maxT = D.lengthSq();

			wo = (r.o - its.itsPoint).normalized();

			if (dot(wo, wi) > 0)
			{

				if (!Utils::hasIntersection(R, objList))
				{

					Vector3D I = lsList[i].getIntensity(its.itsPoint);
					//Vector3D I = Vector3D(1, 1, 1);


					Vector3D R = its.shape->getMaterial().getReflectance(its.normal, wo, wi);

					returnColor += Utils::multiplyPerCanal(I, R);

					//NEW CODE
					//std::cout << R << wo << wi << std::endl;
	
				}

			}
		}

		return returnColor;

	}
	else
	{
		return bgColor;
	}

}
