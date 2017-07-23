#ifndef ANIMATEDLIGHTDATA_H
#define ANIMATEDLIGHTDATA_H

#include "Libs/json.hpp"
#include "Lights/lighttype.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector3.hpp>

using json = nlohmann::json;


struct LightFrame
{
    LightFrame(const json & obj);
    LightFrame(float _time = 1);

    json toJson() const;

    float time;
    sf::Vector3f pos;
    sf::Color color;
    float radius;
    float yaw;
    float pitch; //or angle for spot type
    float intensity;
};

class AnimatedLightData
{
public:
    AnimatedLightData(LightType type);
    AnimatedLightData(const json & obj);

    json toJson() const;

    inline unsigned int frameCount() const { return m_frames.size(); }
    inline const LightFrame & operator[] (unsigned int index ) const { return m_frames[index]; }
    inline LightFrame & operator [] (unsigned int index) { return m_frames[index]; }

    inline std::vector<LightFrame>::const_iterator begin() const {return m_frames.begin(); }
    inline std::vector<LightFrame>::iterator begin() { return m_frames.begin(); }
    inline std::vector<LightFrame>::const_iterator end() const {return m_frames.end(); }
    inline std::vector<LightFrame>::iterator end() { return m_frames.end(); }

    void add(const LightFrame & f);
    void del(unsigned int index);
    inline void clear() { m_frames.clear(); }

    float time() const;
    inline LightType type() const { return m_type; }
    inline void setType(LightType type) { m_type = type;}

    LightFrame at(float t) const;

    static LightFrame defaultFrame();

private:
    unsigned int frameIndex(float &t) const;

    static LightFrame interpolate(const LightFrame & f1, const LightFrame & f2, float lerp);

    template <typename T>
    static T interpolate(const T & a, const T & b, float lerp)
    {
        return a * (1-lerp) + b * lerp;
    }

    LightType m_type;
    std::vector<LightFrame> m_frames;
};

#endif // ANIMATEDLIGHTDATA_H
