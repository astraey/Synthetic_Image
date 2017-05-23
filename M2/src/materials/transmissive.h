
#ifndef TRANSMISSIVE
#define TRANSMISSIVE

#include <iostream>
#include <string>

#include "material.h"


class Transmissive : public Material
{
public:
	Transmissive();
	Transmissive(Vector3D specularColor);

	//Example 
	//Material *green_50 = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 50);

	Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
		const Vector3D &wi) const;
	bool hasSpecular() const;
	bool hasTransmission() const;
	bool hasDiffuseOrGlossy() const;
	double getIndexOfRefraction() const;

private:
	int s;
	Vector3D kd;
	Vector3D ks;
};




#endif