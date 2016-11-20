#include "loadstate.h"
#include "Utilities/tr.h"
#include "Systemes/drawablelist.h"
#include "Machine/statemachine.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "menustate.h"
#include "File/serializer.h"

const float HFrameOffset(5);
const float VFrameOffset(10);
const float TopFrame(-100);
const float OffsetText(20);
const float ButtonOffset(28);

LoadState::LoadState(std::weak_ptr<StateMachine> machine)
    : State(machine)
    , m_titleTexture(tr("res/img/load_en.png"))
    , m_frame("res/img/load_frame.png")
{
    m_title = std::make_shared<sf::Sprite>(*m_titleTexture);
    m_title->setPosition(-m_title->getGlobalBounds().width/2.0f, -m_title->getGlobalBounds().height-120);

    m_returnButton = std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(-30, -10), sf::Vector2f(100, 25), Margin(1), VAlign::V_BOTTOM, HAlign::H_RIGHT), tr("Cancel"));
    m_returnButton->connectClickEvent(std::bind(&onReturn, this));

    createItemsList();
    if(!m_items.empty())
        m_items.front().playButton->changeActiveState(Widget::ControlState::ACTIVE);
    else m_returnButton->changeActiveState(Widget::ControlState::ACTIVE);
}

void LoadState::createItemsList()
{
    Font f("res/font/PressStart2P.ttf");
    auto files(saveList());
    for(const std::string & file : files)
    {
        LoadItem item;
        auto index = m_items.size();
        sf::Vector2f pos(index%2 == 0 ? -HFrameOffset-m_frame->getSize().x : HFrameOffset, (index/2)*(VFrameOffset+m_frame->getSize().y)+TopFrame);
        item.frame = std::make_shared<sf::Sprite>(*m_frame);
        item.frame->setPosition(pos);

        item.saveName = std::make_shared<Label>(AdaptableBounds(sf::Vector2f(pos.x+m_frame->getSize().x/2, pos.y+OffsetText)
                                                                , sf::Vector2f(m_frame->getSize().x-2*OffsetText, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER));
        item.saveName->setText(filenameFromDir(file), f, 8, sf::Color::Black);

        item.playButton = std::make_shared<BasicButton>(AdaptableBounds(sf::Vector2f(pos.x+m_frame->getSize().x/2, pos.y+OffsetText+ButtonOffset)
                                                                        , sf::Vector2f(m_frame->getSize().x-2*OffsetText, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Play"));
        item.removeButton = std::make_shared<BasicButton>(AdaptableBounds(sf::Vector2f(pos.x+m_frame->getSize().x/2, pos.y+OffsetText+2*ButtonOffset)
                                                                          , sf::Vector2f(m_frame->getSize().x-2*OffsetText, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Delete"));

        item.playButton->connectClickEvent(std::bind(&onPressPlayButton, this, file));
        item.removeButton->connectClickEvent(std::bind(&onPressRemoveButton, this, file));

        m_items.push_back(std::move(item));
    }

    for(unsigned int i(0) ; i < m_items.size() ; i++)
    {
        LoadItem & item(m_items[i]);
        item.playButton->connect(MOVE_DOWN, item.removeButton);
        item.removeButton->connect(MOVE_UP, item.playButton);
        if(i > 0 && i % 2 == 1)
        {
            item.playButton->connect(MOVE_LEFT, m_items[i-1].playButton);
            item.removeButton->connect(MOVE_LEFT, m_items[i-1].removeButton);
        }
        if(i < m_items.size()-1 && i % 2 == 0)
        {
            item.playButton->connect(MOVE_RIGHT, m_items[i+1].playButton);
            item.removeButton->connect(MOVE_RIGHT, m_items[i+1].removeButton);
        }

        if(i > 1)
            item.playButton->connect(MOVE_UP, m_items[i-2].removeButton);
        if(i < m_items.size()-2)
            item.removeButton->connect(MOVE_DOWN, m_items[i+2].playButton);
    }

    if(!m_items.empty())
    {
        m_items.back().removeButton->connect(MOVE_DOWN, m_returnButton);
        m_returnButton->connect(MOVE_UP, m_items.back().removeButton);
    }
    if(m_items.size() > 1)
        m_items[m_items.size()-2].removeButton->connect(MOVE_DOWN, m_returnButton);
}

void LoadState::enable()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
        m->setClearColor(sf::Color::White);

    Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(sf::Vector2f(0, 0)));

    DrawableList::add(m_title, 3);

    DrawableList::add(m_returnButton, 1);
    Updatable::add(m_returnButton);
    Controlable::add(m_returnButton);

    for(const auto & item : m_items)
    {
        DrawableList::add(item.frame, 1);
        DrawableList::add(item.saveName, 2);

        DrawableList::add(item.playButton, 2);
        Updatable::add(item.playButton);
        Controlable::add(item.playButton);

        DrawableList::add(item.removeButton, 2);
        Updatable::add(item.removeButton);
        Controlable::add(item.removeButton);
    }
}

void LoadState::disable()
{
    DrawableList::del(m_title);

    DrawableList::del(m_returnButton);
    Updatable::del(m_returnButton);
    Controlable::del(m_returnButton);

    for(const auto & item : m_items)
    {
        DrawableList::del(item.frame);
        DrawableList::del(item.saveName);

        DrawableList::del(item.playButton);
        Updatable::del(item.playButton);
        Controlable::del(item.playButton);

        DrawableList::del(item.removeButton);
        Updatable::del(item.removeButton);
        Controlable::del(item.removeButton);
    }
}

void LoadState::onPressPlayButton(std::string file)
{

}

void LoadState::onPressRemoveButton(std::string file)
{

}

void LoadState::onReturn()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        std::unique_ptr<State> s(std::make_unique<MenuState>(m_machine));
        m->setNext(s);
    }
}

/*
std::shared_ptr<sf::Sprite> m_title;
Texture m_titleTexture;
std::vector<LoadItem> m_items;
std::shared_ptr<BasicButton> m_returnButton;
*/
