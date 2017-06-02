#ifndef DIRECTSHADER_H
#define DIRECTSHADER_H

#include "shader.h"

class DirectShader : public Shader
{
public:
    DirectShader() = delete;
    DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_);

    virtual Vector3D computeColor(const Ray &r,
                             const std::vector<Shape*> &objList,
                             const std::vector<PointLightSource> &lsList) const;

private:
    double maxDist; // Used to store the value of the maximum distance which can be visualized
    Vector3D color; // Used to store the visualization color
};

#endif // DEPTHSHADER_H
