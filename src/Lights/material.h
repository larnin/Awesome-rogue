#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utilities/ressource.h"

struct Material
{
    inline Material()
        : secondaryTexture(defaultNormalTexture)
        , ambiantCoeficient(1)
        , diffuseCoefficient(1)
        , specularCoefficient(0)
        , specularMultiplier(10)
    {

    }

    inline Material(float ambiant, float diffuse, float specular, float multiplier)
        : secondaryTexture(defaultNormalTexture)
        , ambiantCoeficient(ambiant)
        , diffuseCoefficient(diffuse)
        , specularCoefficient(specular)
        , specularMultiplier(multiplier)
    {

    }

    Texture primaryTexture;
    Texture secondaryTexture;

    float ambiantCoeficient;
    float diffuseCoefficient;
    float specularCoefficient;
    float specularMultiplier;


private:
    static Texture defaultNormalTexture;
    static Texture generateDefaultNormalTexture();
};

#endif // MATERIAL_H
