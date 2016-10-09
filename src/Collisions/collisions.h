#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>

class HitBox;
class Line;
class Room;

class Collisions
{
public:
    struct InteractResult
    {
        InteractResult() : collision(false), surfaceAngle(0) {}

        InteractResult(float angle, const sf::Vector2f & pos)
            : collision(true), surfaceAngle(angle), endPos(pos) {}

        bool collision;
        float surfaceAngle;
        sf::Vector2f endPos;
    };

    static InteractResult interact(const HitBox & box1, const sf::Vector2f & pos1, sf::Vector2f speed, const HitBox & box2, const sf::Vector2f & pos2);
    static InteractResult interact(const HitBox & box, const sf::Vector2f & pos, const sf::Vector2f & speed, std::shared_ptr<Room> r);
    static InteractResult interact(const sf::Vector2f & pos1, sf::Vector2f speed, const HitBox & box2, const sf::Vector2f & pos2);
    static InteractResult interact(const sf::Vector2f & pos, const sf::Vector2f & speed, std::shared_ptr<Room> r);
    static bool canRotate(const HitBox & box, const sf::Vector2f & pos, float angle, std::shared_ptr<Room> r);
    static bool intersect(const HitBox & box1, const sf::Vector2f & pos1, const HitBox & box2, const sf::Vector2f & pos2);

private:
    Collisions() = delete;
    ~Collisions() = delete;

    static sf::FloatRect movedBox(sf::FloatRect rect, const sf::Vector2f & speed);
    static std::pair<bool, sf::Vector2f> interactSergment(const Line & l1, const Line & l2);
    static HitBox optimizedBox(const sf::Vector2i & pos, std::shared_ptr<Room> r);
};

#endif // COLLISIONS_H
