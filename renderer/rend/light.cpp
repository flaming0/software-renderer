/*
 * light.cpp
 *
 *  Created on: Mar 10, 2012
 *      Author: flamingo
 */

#include "light.h"

namespace rend
{

const size_t Light::MAX_LIGHTS = 8;
size_t Light::NumLights = 0;

void Light::performShading(ShaderFunction f) const
{
    /*if (!m_isEnabled)
        return;

    math::ivec3 shadedColor;
    list<math::Triangle> &trias = renderlist.triangles();

    foreach (math::Triangle &t, trias)
    {
        Material &material = t.material();

        if (material.shadeMode() == Material::SM_UNDEFINED
                || material.shadeMode() == Material::SM_WIRE)
            continue;

        // FIXME: for all light sources

        shadedColor = shader(material.color(), 1.0);

        if (shadedColor.x > 255) shadedColor.x = 255;
        if (shadedColor.y > 255) shadedColor.y = 255;
        if (shadedColor.z > 255) shadedColor.z = 255;

        material.color() = RgbToInt(shadedColor);

        shadedColor.zero();
    }*/
}

Light::Light(const Color3 &intensity)
    : m_isEnabled(true),
      m_intensity(intensity)
{
    if (NumLights >= MAX_LIGHTS)
    {
        throw LightException("Light limit is reached");
    }

    NumLights++;

    m_shader = &Light::shader;
}

Light::~Light()
{
    NumLights--;
}

math::ivec3 AmbientLight::shader(const Color3 &matColor, double coeff) const
{
    math::ivec3 res;
    res.x = m_intensity.red() * matColor.red() / 256;
    res.y = m_intensity.green() * matColor.green() / 256;
    res.z = m_intensity.blue() * matColor.blue() / 256;

    return res;
}

AmbientLight::AmbientLight(const Color3 &intensity)
    : Light(intensity)
{
}

void AmbientLight::illuminate(RenderList &renderlist) const
{
    if (!m_isEnabled)
        return;

    int shadedColor_r = 0, shadedColor_g = 0, shadedColor_b = 0;
    list<math::Triangle> &trias = renderlist.triangles();

    foreach (math::Triangle &t, trias)
    {
        Material &material = t.material();

        if (material.shadeMode() == Material::SM_UNDEFINED
                || material.shadeMode() == Material::SM_WIRE)
            continue;

        // FIXME: for all light sources

        shadedColor_r += m_intensity.red() * material.color().red() / 256;
        shadedColor_g += m_intensity.green() * material.color().green() / 256;
        shadedColor_b += m_intensity.blue() * material.color().blue() / 256;

        if (shadedColor_r > 255) shadedColor_r = 255;
        if (shadedColor_g > 255) shadedColor_g = 255;
        if (shadedColor_b > 255) shadedColor_b = 255;

        material.color() = RgbToInt(shadedColor_r, shadedColor_g, shadedColor_b);

        shadedColor_r = 0;
        shadedColor_g = 0;
        shadedColor_b = 0;
    }
}

math::ivec3 DirectionalLight::shader(const Color3 &matColor, double coeff) const
{
}

DirectionalLight::DirectionalLight(const Color3 &intensity, const math::vec3 &dir)
    : Light(intensity),
      m_dir(dir)
{
    m_dir.normalize();
}

void DirectionalLight::illuminate(RenderList &renderlist) const
{
    if (!m_isEnabled)
        return;

    int shadedColor_r = 0, shadedColor_g = 0, shadedColor_b = 0;
    list<math::Triangle> &trias = renderlist.triangles();

    foreach (math::Triangle &t, trias)
    {
        Material &material = t.material();

        if (material.shadeMode() == Material::SM_UNDEFINED
                || material.shadeMode() == Material::SM_WIRE)
            continue;

        if (t.normal().isZero())
            continue;

        double dp = t.normal().dotProduct(m_dir);
        if (dp > 0)
        {
            shadedColor_r += m_intensity.red() * dp * material.color().red() / 256;
            shadedColor_g += m_intensity.green() * dp * material.color().green() / 256;
            shadedColor_b += m_intensity.blue() * dp * material.color().blue() / 256;

            if (shadedColor_r > 255) shadedColor_r = 255;
            if (shadedColor_g > 255) shadedColor_g = 255;
            if (shadedColor_b > 255) shadedColor_b = 255;

            material.color() = RgbToInt(shadedColor_r, shadedColor_g, shadedColor_b);
        }

        shadedColor_r = 0;
        shadedColor_g = 0;
        shadedColor_b = 0;
    }
}

math::ivec3 PointLight::shader(const Color3 &matColor, double coeff) const
{
}

PointLight::PointLight(const Color3 &intensity, const math::vec3 &pos,
                       double kc, double kl, double kq)
    : Light(intensity),
      m_pos(pos),
      m_kc(kc),
      m_kl(kl),
      m_kq(kq)
{
}

void PointLight::illuminate(RenderList &renderlist) const
{
    if (!m_isEnabled)
        return;

    int shadedColor_r = 0, shadedColor_g = 0, shadedColor_b = 0;
    list<math::Triangle> &trias = renderlist.triangles();

    foreach (math::Triangle &t, trias)
    {
        Material &material = t.material();

        if (material.shadeMode() == Material::SM_UNDEFINED
                || material.shadeMode() == Material::SM_WIRE)
            continue;

        if (t.normal().isZero())
            continue;

        math::vec3 l = m_pos - t.v(0).p;
        double dist = l.length();

        double dp = t.normal().dotProduct(l);
        if (dp > 0)
        {
            double atten = m_kc + m_kl * dist + m_kq * dist * dist;
            double i = dp / (/*dist*/ atten);

            shadedColor_r += m_intensity.red() * i * material.color().red() / 256;
            shadedColor_g += m_intensity.green() * i * material.color().green() / 256;
            shadedColor_b += m_intensity.blue() * i * material.color().blue() / 256;

            if (shadedColor_r > 255) shadedColor_r = 255;
            if (shadedColor_g > 255) shadedColor_g = 255;
            if (shadedColor_b > 255) shadedColor_b = 255;

            material.color() = RgbToInt(shadedColor_r, shadedColor_g, shadedColor_b);
        }

        shadedColor_r = 0;
        shadedColor_g = 0;
        shadedColor_b = 0;
    }
}

math::ivec3 SpotLight::shader(const Color3 &matColor, double coeff) const
{
}

SpotLight::SpotLight(const Color3 &intensity, const math::vec3 &pos, const math::vec3 &dir,
                     double umbra, double penumbra, double falloff)
    : Light(intensity),
      m_pos(pos),
      m_dir(dir),
      m_innerAngle(umbra),
      m_outerAngle(penumbra),
      m_falloff(falloff)
{
}

void SpotLight::illuminate(RenderList &renderlist) const
{
}

// FIXME: redo this boiler-plate ^
//                               |

}