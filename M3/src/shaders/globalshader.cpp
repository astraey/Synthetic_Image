#include "globalshader.h"
#include "../core/utils.h"

GlobalShader::GlobalShader()
{
}

GlobalShader::GlobalShader(Vector3D bgColor_,int maxDepth_, int ray_) : Shader(bgColor_)
{
	this->bgColor = bgColor_;
	this->maxDepth = maxDepth_;
	this->ray =  ray_;
}

Vector3D GlobalShader::computeColor(const Ray & r, const std::vector<Shape*>& objList,
	const std::vector<PointLightSource>& lsList) const
{
	Intersection its;

	//We return bgColor if there are no intersections between the ray and any object from objList
	if (!Utils::getClosestIntersection(r, objList, its)) return bgColor;

	Vector3D wr, wo = -r.d.normalized();
	Vector3D returnColor = Vector3D(0, 0, 0), indirect = Vector3D(0, 0, 0);
	Vector3D at = Vector3D(0.1, 0.1, 0.1);
	Ray R;

	//PHONG
	if (its.shape->getMaterial().hasDiffuseOrGlossy())
	{
		// COmpute Direct Illumination
		for (int i = 0; i < lsList.size(); i++)
		{
			Vector3D D = lsList[i].getPosition() - its.itsPoint;
			wr = D.normalized();
			R = Ray(its.itsPoint, wr);
			R.maxT = D.length();
			if (!Utils::hasIntersection(R, objList)) {
				if (dot(its.normal, wr) > 0)
				{
					Vector3D I = lsList[i].getIntensity(its.itsPoint);
					Vector3D R = its.shape->getMaterial().getReflectance(its.normal, wo, wr);
					returnColor += Utils::multiplyPerCanal(I, R);
				}
			}
		}

		//Indirect Light Computations

		//Cast of the rays
		if (r.depth == 0) {
			for (int i = 0; i < ray; i++) {
				wr = sampler.getSample(its.normal);
				Ray emitedRay = Ray(its.itsPoint, wr, r.depth + 1);
				Vector3D I = computeColor(emitedRay, objList, lsList);
				Vector3D R = its.shape->getMaterial().getReflectance(its.normal, wr, wo);
				indirect += Utils::multiplyPerCanal(I,R);
			}
				indirect /= ray;
		}

		//Bounce Control
		else if (r.depth >= maxDepth) {
			Vector3D kd = its.shape->getMaterial().getDiffuseCoefficient();
			indirect = Utils::multiplyPerCanal(kd, at);
		}

		//Color Computation
		else {
			Ray rn = Ray(its.itsPoint, its.normal.normalized(), r.depth + 1);
			Ray rr = Ray(its.itsPoint, Utils::computeReflectionDirection(r.d, its.normal.normalized()),
				r.depth + 1);
			indirect = (computeColor(rn, objList, lsList) + computeColor(rr, objList, lsList)) *.5;
		}
		return returnColor + indirect;
	}

	//MIRROR
	else if (its.shape->getMaterial().hasSpecular()) {

		if (dot(its.normal, wo) < 0) return bgColor;

		wr = Utils::computeReflectionDirection(r.d, its.normal);
		Ray reflectedRay = Ray(its.itsPoint, wr, r.depth);
		return computeColor(reflectedRay, objList, lsList);

	}

	//Teransmissive
	else if (its.shape->getMaterial().hasTransmission()) {
		double ETA, cosThetaI, cosThetaT;
		Vector3D normal = its.normal.normalized();

		cosThetaI = dot(normal, wo);

		ETA = its.shape->getMaterial().getIndexOfRefraction();

		if (cosThetaI < 0) {
			ETA = 1 / ETA;
			normal = -normal;
			cosThetaI = dot(wo, normal);
		}


		if (!Utils::isTotalInternalReflection(ETA, cosThetaI, cosThetaT))
			wr = Utils::computeTransmissionDirection(r, normal, ETA, cosThetaI, cosThetaT);

		else
			wr = Utils::computeReflectionDirection(r.d, normal);

		Ray refractedRay = Ray(its.itsPoint, wr, r.depth);
		return computeColor(refractedRay, objList, lsList);
	}

}
