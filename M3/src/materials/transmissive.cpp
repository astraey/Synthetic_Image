#include "transmissive.h"

Transmissive::Transmissive(double eta_, Vector3D spec)
{

	this->refractionIndex = eta_;
	this->specular = spec;

}

Vector3D Transmissive::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	return  n * dot(n, wo) * 2.0 - wo;
}

bool Transmissive::hasSpecular() const
{
	return false;
}

bool Transmissive::hasTransmission() const
{
	return true;
}

bool Transmissive::hasDiffuseOrGlossy() const
{
	return false;
}

double Transmissive::getIndexOfRefraction() const
{
	return this->refractionIndex;
}