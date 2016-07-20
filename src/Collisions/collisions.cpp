#include <array>
#include "collisions.h"
#include "Utilities/vect2convert.h"
#include "Map/blocktype.h"
#include "Map/room.h"
#include "hitbox.h"

const float epsilon(0.01f);

Collisions::InteractResult Collisions::interact(const HitBox & box1, const sf::Vector2f & pos1, sf::Vector2f speed
                                                , const HitBox & box2, const sf::Vector2f & pos2)
{
    speed = normalise(speed)*(norm(speed)+epsilon);

    sf::FloatRect rect1(box1.globalRect());
    rect1.left += pos1.x;
    rect1.top += pos1.y;
    sf::FloatRect rect2(box2.globalRect());
    rect2.left += pos2.x;
    rect2.top += pos2.y;
    if(!movedBox(rect1, speed).intersects(rect2))
        return Collisions::InteractResult();

    Collisions::InteractResult endMove;

    for(const auto & l1 : box1.m_lines)
    {
        Line l11(l1.move(pos1));
        for(const auto & l2 : box2.m_lines)
        {
            Line l22(l2.move(pos2));

            auto lambda([speed](const Line & l1, const Line & l2, const sf::Vector2f & plot)
            {
                const auto v(interactSergment(l1, l2));
                if(!v.first)
                    return norm(speed);
                return norm(plot-v.second);
            });

            std::array<float, 4> result
            {lambda(Line(l11.pos1, l11.pos1+speed), l22, l11.pos1),
             lambda(Line(l11.pos2, l11.pos2+speed), l22, l11.pos2),
             lambda(Line(l22.pos1, l22.pos1-speed), l11, l22.pos1),
             lambda(Line(l22.pos2, l22.pos2-speed), l11, l22.pos2)};

            for(unsigned int i(0) ; i < result.size() ; i++)
            {
                if(result[i] >= norm(speed))
                    continue;
                if(!endMove.collision || norm(pos1-endMove.endPos) > result[i])
                {
                    endMove.collision = true;
                    endMove.endPos = pos1+normalise(speed)*result[i];
                    endMove.surfaceAngle = angle(i >= 2 ? l11.pos2-l11.pos1 : l22.pos2-l22.pos1);
                }
            }
        }
    }

    sf::Vector2f vect(endMove.endPos-pos1);
    if(norm(vect) > epsilon)
        endMove.endPos = pos1 + normalise(vect)*(norm(vect)-epsilon);
    else endMove.endPos = pos1;

    return endMove;
}

Collisions::InteractResult Collisions::interact(const HitBox & box, const sf::Vector2f & pos, const sf::Vector2f & speed, std::shared_ptr<Room> r)
{
    if(!r)
    return Collisions::InteractResult();

    sf::FloatRect boxRect(box.globalRect());
    boxRect.left += pos.x;
    boxRect.top += pos.y;
    sf::FloatRect movedBoxRect(movedBox(boxRect, speed));

    Collisions::InteractResult result;

    for(int i(movedBoxRect.left - (movedBoxRect.left<0)) ; i < movedBoxRect.left+movedBoxRect.width + 1 ; i++)
        for(int j(movedBoxRect.top - (movedBoxRect.top<0)) ; j < movedBoxRect.top + movedBoxRect.height + 1 ; j++)
        {
            HitBox boxBlock(optimizedBox(sf::Vector2i(i, j), r));
            if(boxBlock.m_lines.empty())
                continue;

            Collisions::InteractResult localResult(interact(box, pos, speed, boxBlock, sf::Vector2f(i, j)));
            if(!result.collision)
                result = localResult;
            else if(norm(pos-result.endPos) > norm(pos - localResult.endPos))
                result = localResult;
        }
    return result;
}

Collisions::InteractResult Collisions::interact(const sf::Vector2f & pos1, sf::Vector2f speed, const HitBox & box2, const sf::Vector2f & pos2)
{
    speed = normalise(speed)*(norm(speed)+epsilon);

    sf::FloatRect rect1(std::min(pos1.x, pos1.x+speed.x), std::min(pos1.y, pos1.y+speed.y), std::abs(speed.x), std::abs(speed.y));
    sf::FloatRect rect2(box2.globalRect());
    rect2.left += pos2.x;
    rect2.top += pos2.y;
    if(!rect1.intersects(rect2))
        return Collisions::InteractResult();

    Collisions::InteractResult endMove;

    Line l11(pos1, pos1+speed);
    for(const auto & l2 : box2.m_lines)
    {
        Line l22(l2.move(pos2));

        const auto v(interactSergment(l11, l22));
        if(!v.first)
            continue;
        if(!endMove.collision || norm(pos1-v.second) < norm(pos1-endMove.endPos))
        {
            endMove.collision = true;
            endMove.endPos = v.second;
            endMove.surfaceAngle = angle(l22.pos2-l22.pos1);
        }
    }


    sf::Vector2f vect(endMove.endPos-pos1);
    if(norm(vect) > epsilon)
        endMove.endPos = pos1 + normalise(vect)*(norm(vect)-epsilon);
    else endMove.endPos = pos1;

    return endMove;
}

Collisions::InteractResult Collisions::interact(const sf::Vector2f & pos, const sf::Vector2f & speed, std::shared_ptr<Room> r)
{
    if(!r)
        return Collisions::InteractResult();

    sf::Vector2f posMin(std::min(pos.x, pos.x+speed.x), std::min(pos.y, pos.y+speed.y));
    sf::Vector2f absSpeed(std::abs(speed.x), std::abs(speed.y));

    sf::Vector2i posMinI(posMin.x -(posMin.x < 0), posMin.y - (posMin.y < 0));

    Collisions::InteractResult result;

    for(int i(0) ; i < absSpeed.x+1 ; i++)
        for(int j(0) ; j < absSpeed.y+1 ; j++)
        {
            HitBox box(optimizedBox(sf::Vector2i(i, j)+posMinI, r));
            if(box.m_lines.empty())
                continue;

            Collisions::InteractResult localResult(Collisions::interact(pos, speed, box, sf::Vector2f(i, j)+sf::Vector2f(posMinI)));
            if(!localResult.collision)
                continue;
            if(!result.collision)
            {
                result = localResult;
                continue;
            }
            if(norm(pos-result.endPos)>norm(pos-localResult.endPos))
                result = localResult;
        }
    return result;
}

bool Collisions::canRotate(const HitBox & box, const sf::Vector2f & pos, float angle, std::shared_ptr<Room> r)
{
    if(!r)
        return true;

    HitBox boxResult = box.transform(angle, false, false);
    HitBox rotatedBox;
    for(unsigned int i(0) ; i < box.m_lines.size() ; i++)
    {
        rotatedBox.addLine(Line(box.m_lines[i].pos1, boxResult.m_lines[i].pos1));
        rotatedBox.addLine(Line(box.m_lines[i].pos2, boxResult.m_lines[i].pos2));
    }

    sf::FloatRect rect1(box.globalRect());
    sf::FloatRect rect2(boxResult.globalRect());
    sf::Vector2f min(std::min(rect1.left, rect2.left), std::min(rect1.top, rect2.top));
    sf::Vector2f max(std::max(rect1.left+rect1.width, rect2.left+rect2.width), std::min(rect1.top+rect1.height, rect2.top+rect2.height));
    sf::FloatRect rectGlobal(min+pos, max-min);

    for(int i(rectGlobal.left) ; i < rectGlobal.left+rectGlobal.width + 1 ; i++)
        for(int j(rectGlobal.top) ; j < rectGlobal.top + rectGlobal.height + 1 ; j++)
        {
            HitBox boxBlock;
            if(i < 0 || i >= int(r->getSize().x) || j < 0 || j >= int(r->getSize().y))
                boxBlock = BlockType::createBox(createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL));
            else
            {
                Block & b ((*r)(sf::Vector2u(i, j)));
                if(getBoxType(b.boxCaracts) == BoxType::EMPTY)
                    continue;
                boxBlock = BlockType::createBox(b.boxCaracts);
            }

            if(intersect(boxBlock, sf::Vector2f(i, j), boxResult, pos))
                return false;
            if(intersect(boxBlock, sf::Vector2f(i, j), rotatedBox, pos))
                return false;
        }
    return true;
}

bool Collisions::intersect(const HitBox & box1, const sf::Vector2f & pos1, const HitBox & box2, const sf::Vector2f & pos2)
{
    for(const auto & line1 : box1.m_lines)
    {
        Line l1(line1.move(pos1));
        for(const auto & line2 : box2.m_lines)
        {
            Line l2(line2.move(pos2));
            if(interactSergment(l1, l2).first)
                return true;
        }
    }
    return false;
}

sf::FloatRect Collisions::movedBox(sf::FloatRect rect, const sf::Vector2f & speed)
{
    rect.width += std::abs(speed.x);
    rect.height += std::abs(speed.y);
    if(speed.x < 0)
        rect.left += speed.x;
    if(speed.y < 0)
        rect.top += speed.y;
    return rect;
}

std::pair<bool, sf::Vector2f> Collisions::interactSergment(const Line & l1, const Line & l2)
{
    sf::Vector2f sPos(l1.pos2-l1.pos1), sSeg(l2.pos2-l2.pos1);
    float denom(sPos.x*sSeg.y - sPos.y*sSeg.x);
    if(denom == 0)
        return std::make_pair<bool, sf::Vector2f>(false, sf::Vector2f());

    float u((l1.pos1.x*sSeg.y - l2.pos1.x*sSeg.y - sSeg.x*l1.pos1.y + sSeg.x*l2.pos1.y)/denom);
    float v((- sPos.x*l1.pos1.y + sPos.x*l2.pos1.y + sPos.y*l1.pos1.x - sPos.y*l2.pos1.x)/denom);
    if(u >= -1 && u <= 0 && v >= -1 && v <= 0)
        return std::make_pair<bool, sf::Vector2f>(true, l1.pos1 - sPos*u);
    return std::make_pair<bool, sf::Vector2f>(false, sf::Vector2f());
}

HitBox Collisions::optimizedBox(const sf::Vector2i & pos, std::shared_ptr<Room> r)
{
    if(!r)
        return HitBox();

    HitBox box;
    if(pos.x < 0 || pos.y < 0 || pos.x >= int(r->getSize().x) || pos.y >= int(r->getSize().y))
        box = BlockType::createBox(createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL));
    else box = BlockType::createBox((*r)(sf::Vector2u(pos)).boxCaracts);

    if(box.m_lines.empty())
        return box;

    std::array<sf::Vector2i, 4> tests { pos + sf::Vector2i(0, 1),
                                        pos + sf::Vector2i(1, 0),
                                        pos + sf::Vector2i(0, -1),
                                        pos + sf::Vector2i(-1, 0)};
    HitBox returnBox;

    for(const sf::Vector2i & p : tests)
    {
        HitBox blockTest;
        if(p.x < 0 || p.y < 0 || p.x >= int(r->getSize().x) || p.y >= int(r->getSize().y))
            blockTest = BlockType::createBox(createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL));
        else blockTest = BlockType::createBox((*r)(sf::Vector2u(p)).boxCaracts);

        for(const Line & l1 : box.m_lines)
        { bool canAdd(true);
            for(const Line & l2 : blockTest.m_lines)
            {
                if(l1.move(sf::Vector2f(pos)).sameAs(l2.move(sf::Vector2f(p)), epsilon))
                {
                    canAdd = false;
                    break;
                }
            }
            if(canAdd)
                returnBox.addLine(l1);
        }
        box = returnBox;
        returnBox = HitBox();
    }

    return box;
}
