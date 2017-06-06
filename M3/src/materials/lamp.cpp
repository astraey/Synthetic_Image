#include "lamp.h"

Lamp::Lamp(double eta_, Vector3D spec)
{

	this->refractionIndex = eta_;
	this->specular = spec;

}

Vector3D Lamp::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const
{
	return  n * dot(n, wo) * 2.0 - wo;
}

bool Lamp::hasSpecular() const
{
	return false;
}
bool Lamp::hasTransmission() const
{
	return true;
}
bool Lamp::hasDiffuseOrGlossy() const
{
	return true;
}
double Lamp::getIndexOfRefraction() const
{
	return this->refractionIndex;
}