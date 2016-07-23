#ifndef CAMERAEFFECT_H
#define CAMERAEFFECT_H

#include "Systemes/updatable.h"
#include "Utilities/noncopiable.h"
#include <SFML/System/Vector2.hpp>

class CameraEffect : public Updatable, private NonCopiable
{
public:
    CameraEffect()
        : m_orientation(0), m_zoom(1), m_offset(0, 0) {}
    CameraEffect(CameraEffect &&) = default;
    CameraEffect & operator =(CameraEffect &&) = default;
    virtual ~CameraEffect() = default;

    float getOrientation() const
    {
        return m_orientation;
    }

    float getZoom() const
    {
        return m_zoom;
    }

    sf::Vector2f getOffset() const
    {
        return m_offset;
    }

protected:
    float m_orientation;
    float m_zoom;
    sf::Vector2f m_offset;
};

#endif // CAMERAEFFECT_H
