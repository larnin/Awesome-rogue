#include "interactor.h"
#include "Map/room.h"
#include "Events/Datas/eventinteraction.h"
#include "Events/event.h"
#include "Entities/Types/entity.h"
#include "Map/location.h"
#include "Controles/commandsvalue.h"

Interactor::Interactor(std::weak_ptr<Entity> holder)
    : Controlable(ControlState::ACTIVE)
    , m_holder(holder)
    , m_text(V_CENTER, H_CENTER)
    , m_font("res/font/PressStart2P.ttf")
{
    m_text.setFont(m_font);
    m_text.setTextSize(8);
    m_text.setTextColor(sf::Color::White);
}

void Interactor::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    const float verticalOffset(16.0f);
    const sf::Vector2f size(128, 16);
    std::shared_ptr<Entity> e(m_holder.lock());
    if(!e)
        return;

    auto interaction(getInteractable());
    if(interaction.first == BI_NO_INTERACTION)
        return ;

    m_text.setText(BlockType::interactionName(interaction.first));
    sf::Vector2f pos(sf::Vector2f(interaction.second.toGlobalPos())*float(BlockType::tileSize));
    m_text.draw(target, sf::FloatRect(pos.x-size.x/2, pos.y-size.y/2-verticalOffset, size.x, size.y));
}

void Interactor::control(CommandsValue & v)
{
    std::shared_ptr<Entity> e(m_holder.lock());
    if(!e)
        return;

    if(v.isPressEvent(KEY_ACTION))
    {
        auto interaction(getInteractable());
        if(interaction.first != BI_NO_INTERACTION)
            Event<EventInteraction>::send(EventInteraction(interaction.second, interaction.first));
    }
}

std::pair<BlockInteractionType, Location> Interactor::getInteractable() const
{
    const auto emptyReturn(std::make_pair<BlockInteractionType, sf::Vector2u>(BI_NO_INTERACTION, sf::Vector2u(0, 0)));
    std::shared_ptr<Entity> e(m_holder.lock());
    if(!e)
        return emptyReturn;
    std::shared_ptr<Room> r(e->getPos().getRoom());
    if(!r)
        return emptyReturn;

    for(int i(-1) ; i <= 1 ; i++)
        for(int j(-1) ; j <= 1 ; j++)
        {
            sf::Vector2i pos(e->getPos().getBlockPos().x+i, e->getPos().getBlockPos().y+j);
            if(pos.x < 0 || pos.y < 0 || pos.x >= int(r->getSize().x) || pos.y >= int(r->getSize().y))
                continue;
            BlockInteractionType type(BlockType::getInteraction(r->get(sf::Vector2u(pos))));
            if(type != BI_NO_INTERACTION)
                return std::make_pair(type, Location(pos, r));
        }
    return emptyReturn;
}
