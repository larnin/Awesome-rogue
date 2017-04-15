#ifndef SHOWFPS_H
#define SHOWFPS_H

#include "Utilities/ressource.h"
#include "textDrawer.h"
#include "Systemes/updatable.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Clock.hpp>

class ShowFPS : public sf::Drawable, public Updatable
{
public:
    ShowFPS();
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;
    void update(const sf::Time & elapsedTime) override;

private:
    float m_time;
    unsigned int m_frameCount;
    Font m_font;
    TextDrawer m_text;
};

#endif // SHOWFPS_H
