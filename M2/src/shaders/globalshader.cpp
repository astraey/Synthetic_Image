#include "globalshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.cpp"

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
	Vector3D wo = - r.d;

	Vector3D returnColor = bgColor;

	Vector3D Indirect = Vector3D(0,0,0);

	if (Utils::getClosestIntersection(r, objList, its))
	{

		//Phong
		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {

			// COmpute Direct Illumination
			for (int i = 0; i < nL; i++)
			{
				Vector3D D = lsList[i].getPosition() - its.itsPoint;
				wi = D.normalized();
				R = Ray(its.itsPoint, wi);
				R.maxT = D.length(); 
				if (!Utils::hasIntersection(R, objList)) {
					if (dot(its.normal, wi) > 0)
					{
						Vector3D I = lsList[i].getIntensity(its.itsPoint);
						Vector3D R = its.shape->getMaterial().getReflectance(its.normal, wo, wi);
						returnColor += Utils::multiplyPerCanal(I, R);
					}
				}
			}

			// COmpute indirect illumination
			if (r.depth > 0) {
				Indirect = Utils::multiplyPerCanal(this->at, its.shape->getMaterial().getDiffuseCoefficient());
				//return returnColor + Indirect;
			}
			else if (r.depth == 0) {
				for (int i = 0; i < r.maxT; i++) {
					HemisphericalSampler sampler;
					Vector3D randomDirection = sampler.getSample(its.normal); //direccion aleatoria
					Ray R0 = Ray(its.itsPoint, randomDirection, r.depth + 1);
					Vector3D I = computeColor(R0,objList,lsList);
					Vector3D R = its.shape->getMaterial().getReflectance(its.normal,randomDirection, wo);
				    Indirect += Utils::multiplyPerCanal(I, R); // multiplicar color de direccion aleatoria por wo
					
				}
				Indirect /= r.maxT;
			}
			return returnColor + Indirect;
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
