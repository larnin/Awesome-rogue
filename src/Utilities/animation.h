#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics/Rect.hpp>
#include <vector>

struct Frame
{
    Frame()
        : time(0)
    {}

    Frame(float _time, const sf::FloatRect & _texRect, const sf::Vector2f & _offset)
        : time(_time), texRect(_texRect), offset(_offset)
    {}

    float time;
    sf::FloatRect texRect;
    sf::Vector2f offset;
};

class Animation
{
public:
    Animation(bool singleShoot = false);

    void addFrame(const Frame & f);

    bool isSingleShoot() const;
    void setSingleShoot(bool value);
    bool finished() const;
    void restart();

    Frame getCurrent() const;
    Frame getAt(float time) const;

    void setTime(float time);
    void addTime(float time);
    float getCurrentTime() const;
    float getAnimationTime() const;

private:
    std::vector<Frame> m_frames;
    bool m_singleShoot;
    bool m_finished;
    float m_currentTime;
    float m_totalTime;
};

#endif // ANIMATION_H
