#include "phong.h"

Phong::Phong(Vector3D difuseColor_, Vector3D specularColor_, int s_)
{

	s = s_;
	Vector3D kd = difuseColor_;
	Vector3D ks = specularColor_;

}


Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const
{
	Vector3D wr = n.operator*(dot(n, wi)).operator*(2).operator-(wi);
	Vector3D reflectance = kd.operator*(dot(wi, n)).operator+(ks.operator*(std::pow(dot(wo, wr), s)));
	return reflectance;
}

bool Phong::hasSpecular() const
{
	return false;
}
bool Phong::hasTransmission() const
{
	return false;
}
bool Phong::hasDiffuseOrGlossy() const
{
	return true;
}
double Phong::getIndexOfRefraction() const
{
	return true;
}