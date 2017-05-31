#include "globalshader.h"
#include "../core/utils.h"

GlobalShader::GlobalShader()
{
}

GlobalShader::GlobalShader(Vector3D bgColor_, int rays_, int maxDepth_) : bgColor(bgColor_), rays(rays_), maxDepth(maxDepth_)
{
}

Vector3D GlobalShader::computeColor(const Ray & r, const std::vector<Shape*>& objList,
	const std::vector<PointLightSource>& lsList) const
{
	Intersection its;

	if (!Utils::getClosestIntersection(r, objList, its)) return bgColor;

	Vector3D color, indirect, wr, wo = -r.d.normalized();

	//PHONG
	if (its.shape->getMaterial().hasDiffuseOrGlossy())
	{
		for (int i = 0; i < lsList.size(); i++)
		{
			Vector3D direction(lsList[i].getPosition() - its.itsPoint);
			double distanceLS = direction.length();
			Vector3D wi = direction.normalized();

			Ray shadowRay(its.itsPoint, wi);
			shadowRay.maxT = distanceLS;

			if (!Utils::hasIntersection(shadowRay, objList) && dot(its.normal, wi) > 0) {
				color += Utils::multiplyPerCanal(its.shape->getMaterial().getReflectance(its.normal, wo, wi),
					lsList[i].getIntensity(its.itsPoint));
			}
		}


		//Indirect Light

		//First Iteraction
		if (r.depth == 0) {
			for (int i = 0; i < rays; i++) {
				wr = sampler.getSample(its.normal);
				Ray emmitedRay = Ray(its.itsPoint, wr, r.depth + 1);
				indirect += Utils::multiplyPerCanal(computeColor(emmitedRay, objList, lsList),
					its.shape->getMaterial().getReflectance(its.normal, wr, wo));
			}
			indirect /= rays;
		}

		else if (r.depth >= maxDepth) {
			Vector3D at = Vector3D(0.1, 0.1, 0.1);
			Vector3D kd = its.shape->getMaterial().getDiffuseCoefficient();
			indirect = Utils::multiplyPerCanal(kd, at);
		}

		else {
			Ray rn = Ray(its.itsPoint, its.normal.normalized(), r.depth + 1);
			Ray rr = Ray(its.itsPoint, Utils::computeReflectionDirection(r.d, its.normal.normalized()),
				r.depth + 1);
			indirect = (computeColor(rn, objList, lsList) + computeColor(rr, objList, lsList)) *.5;
		}

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
		double eta, cosThetaI, cosThetaT_out;
		Vector3D normal = its.normal.normalized();

		cosThetaI = dot(normal, wo);

		eta = its.shape->getMaterial().getIndexOfRefraction();

		if (cosThetaI < 0) {
			eta = 1 / eta;
			normal = -normal;
			cosThetaI = dot(wo, normal);
		}


		if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT_out))
			wr = Utils::computeTransmissionDirection(r, normal, eta, cosThetaI, cosThetaT_out);

		else
			wr = Utils::computeReflectionDirection(r.d, normal);

		Ray refractedRay = Ray(its.itsPoint, wr, r.depth);
		return computeColor(refractedRay, objList, lsList);
	}


	return color + indirect;
}
