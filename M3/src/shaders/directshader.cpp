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
	Vector3D wo = -r.d.normalized();
	Vector3D wr = -r.d.normalized();

	Vector3D returnColor = bgColor;

	if (Utils::getClosestIntersection(r, objList, its))
	{

		//PHONG
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

			wr = its.normal*(dot(wo, its.normal)) * 2 - wo;
			Ray R2 = Ray(its.itsPoint, wr);
			return computeColor(R2, objList, lsList);

		}

		//TRANSMISSIVE
		else if (its.shape->getMaterial().hasTransmission()) {
			
			Vector3D normal = its.normal;
			double cosThetaI = dot(normal, wo);
			double eta = its.shape->getMaterial().getIndexOfRefraction();
			double cosThetaT_out;

			if (cosThetaI < 0) {
				eta = 1 / eta;
				normal = -normal;
				cosThetaI = dot(wo, normal);
			}


			if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT_out))
				wr = Utils::computeTransmissionDirection(r, normal, eta, cosThetaI, cosThetaT_out);

			else
				wr = Utils::computeReflectionDirection(r.d, normal);

			Ray refRay = Ray(its.itsPoint, wr, r.depth + 1);
			return computeColor(refRay, objList, lsList);
		}

	}

	else
	{
		return bgColor;
	}
}
