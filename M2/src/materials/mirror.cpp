#include "mirror.h"

Mirror::Mirror(Vector3D specularColor_)
{

	this->ks = specularColor_;
	//WE ARE RETURNING THIS
	this->kd = Vector3D(0,0,255);

}


Vector3D Mirror::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const
{

	//std::cout << n << wo << wi << this->kd << this->ks << this->s << std::endl;
	//Vector3D wr = n.operator*(dot(n, wi)).operator*(2).operator-(wi);
	Vector3D wr = n.operator*(dot(n, wi)) * 2 -wi;

	//Vector3D reflectance = kd.operator*(dot(wi, n)).operator+(ks.operator*(std::pow(dot(wo, wr), s)));
	Vector3D reflectance = this->kd.operator*(dot(wi, n)) + this->ks*(std::pow(dot(wo, wr), this->s));
	//std::cout << reflectance << std::endl;
	return reflectance;
}

Vector3D Mirror::getDiffuseCoefficient() const
{
	return this->kd;
}

bool Mirror::hasSpecular() const
{
	return true;
}
bool Mirror::hasTransmission() const
{
	return false;
}
bool Mirror::hasDiffuseOrGlossy() const
{
	return false;
}
double Mirror::getIndexOfRefraction() const
{
	return true;
}