#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "material.h"
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <vector>

class PhongShader
{
public:
    PhongShader();
    inline void setMaterial(const Material & m){m_material = m;}
    const sf::Shader & get();

    static void initialize();
    static void setPointLights(const std::vector<sf::Glsl::Vec3> & positions, const std::vector<sf::Glsl::Vec4> & colors, const std::vector<float> & radius);
    static void setAmbiant(const sf::Color & c);

private:
    void applyMaterial();
    Material m_material;

    static sf::Shader m_shader;
    static const unsigned int maxLight;
};

#endif // PHONGSHADER_H
