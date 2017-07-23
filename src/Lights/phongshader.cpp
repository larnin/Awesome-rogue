#include "phongshader.h"

#include <iostream>

sf::Shader PhongShader::m_shader;
const unsigned int PhongShader::maxLight(100);

PhongShader::PhongShader()
{

}

const sf::Shader & PhongShader::get() const
{
    applyMaterial();
    return m_shader;
}

void PhongShader::applyMaterial() const
{
    m_shader.setUniform("primaryTexture", *m_material.primaryTexture);
    m_shader.setUniform("secondaryTexture", *m_material.secondaryTexture);
    m_shader.setUniform("material", sf::Glsl::Vec4(m_material.ambiantCoeficient, m_material.diffuseCoefficient, m_material.specularCoefficient, m_material.specularMultiplier));
}

void PhongShader::setLights(const std::vector<sf::Glsl::Vec4> & lightsColor
                            , const std::vector<sf::Glsl::Vec3> & lightsPos
                            , const std::vector<float> & lightsType
                            , const std::vector<sf::Glsl::Vec4> & lightsParams)
{
    unsigned int size(std::min({lightsColor.size(), lightsPos.size(), lightsType.size(), lightsParams.size()}));
    m_shader.setUniformArray("light", lightsPos.data(), lightsPos.size());
    m_shader.setUniformArray("lightColor", lightsColor.data(), lightsColor.size());
    m_shader.setUniformArray("lightType", lightsType.data(), lightsType.size());
    m_shader.setUniformArray("lightParams", lightsParams.data(), lightsParams.size());
    m_shader.setUniform("lightCount", (int)size);
}

void PhongShader::initialize()
{
    m_shader.loadFromFile("res/shader/phong/2dmultilight.vert", "res/shader/phong/2dmultilight.frag");
    m_shader.setUniform("lightCount", 0);
    setAmbiant(sf::Color::White);
}

void PhongShader::setAmbiant(const sf::Color & c)
{
    m_shader.setUniform("ambiantColor", sf::Glsl::Vec4(c));
}
