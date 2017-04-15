#ifndef BASICTEXTBOX_H
#define BASICTEXTBOX_H

#include "abstracttextbox.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/RenderTexture.hpp>

class BasicTextBox : public AbstractTextBox
{
public:
    BasicTextBox(const AdaptableBounds & _bounds = AdaptableBounds());
    ~BasicTextBox() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;
    virtual void update(const sf::Time & t) override;

private:
    void updateTextTexture(const sf::Vector2f & size, bool drawCursor) const;

    Texture m_texture;
    Font m_font;
    sf::Time m_cursorTimer;
    mutable sf::RenderTexture m_textTexture;
};

#endif // BASICTEXTBOX_H
