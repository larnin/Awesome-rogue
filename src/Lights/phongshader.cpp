#include "phongshader.h"

sf::Shader PhongShader::m_shader;
const unsigned int PhongShader::maxLight(100);

PhongShader::PhongShader()
{

}

const sf::Shader & PhongShader::get()
{
    applyMaterial();
    return m_shader;
}

void PhongShader::applyMaterial()
{
    m_shader.setUniform("primaryTexture", *m_material.primaryTexture);
    m_shader.setUniform("secondaryTexture", *m_material.secondaryTexture);
    m_shader.setUniform("material", sf::Glsl::Vec4(m_material.ambiantCoeficient, m_material.diffuseCoefficient, m_material.specularCoefficient, m_material.specularMultiplier));
}

void PhongShader::setPointLights(const std::vector<sf::Glsl::Vec3> & positions, const std::vector<sf::Glsl::Vec4> & colors, const std::vector<float> & radius)
{
    unsigned int size(std::min(std::min(maxLight, positions.size()), std::min(colors.size(), radius.size())));
    m_shader.setUniformArray("lights", positions.data(), size);
    m_shader.setUniformArray("lightColor", colors.data(), size);
    m_shader.setUniformArray("lightRadius", radius.data(), size);
    m_shader.setUniform("lightCount", (int)size);
}

void PhongShader::initialize()
{
    m_shader.loadFromFile("res/shader/phong/2dmultilight.vert", "res/shader/phong/2dmultilight.frag");
    m_shader.setUniform("lightCount", 0);
    m_shader.setUniform("ambiantColor", sf::Glsl::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void PhongShader::setAmbiant(const sf::Color & c)
{
    m_shader.setUniform("ambiantColor", sf::Glsl::Vec4(c.r, c.g, c.b, c.a));
}
