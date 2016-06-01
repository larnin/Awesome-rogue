#ifndef ABSTACTRADIOBUTTONITEM_H
#define ABSTACTRADIOBUTTONITEM_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

class AbstactRadioButtonItem
{
public:
    AbstactRadioButtonItem();
    AbstactRadioButtonItem(AbstactRadioButtonItem &&) = default;
    AbstactRadioButtonItem & operator=(AbstactRadioButtonItem &&) = default;
    virtual ~AbstactRadioButtonItem() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states, const sf::FloatRect & bounds) const = 0;

    bool checked;
    bool selected;
    std::string label;
};

#endif // ABSTACTRADIOBUTTONITEM_H
