#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H

#include "shader.h"

class GlobalShader : public Shader
{
public:
	GlobalShader() = delete;
	GlobalShader(Vector3D color_, double maxDist_, Vector3D bgColor_, Vector3D at_);

    virtual Vector3D computeColor(const Ray &r,
                             const std::vector<Shape*> &objList,
                             const std::vector<PointLightSource> &lsList) const;

private:
    double maxDist; // Used to store the value of the maximum distance which can be visualized
    Vector3D color; // Used to store the visualization color
	Vector3D at;
};

#endif // DEPTHSHADER_H
