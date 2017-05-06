#include "loadsavestate.h"
#include "Utilities/tr.h"
#include "Systemes/drawablelist.h"
#include "Machine/statemachine.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "GUI/Widgets/Buttons/basicbutton.h"
#include "GUI/Widgets/TextBoxs/basictextbox.h"
#include "GUI/Widgets/label.h"
#include "menustate.h"
#include "gamestate.h"
#include "File/serializer.h"

const float HFrameOffset(5);
const float VFrameOffset(10);
const float TopFrame(-100);
const float OffsetText(20);
const float ButtonOffset(28);

LoadSaveState::LoadSaveState(std::weak_ptr<StateMachine> machine, bool load)
    : State(machine)
    , m_titleTexture(load ? tr("res/img/load_en.png") : tr("res/img/save_en.png"))
    , m_frame("res/img/load_frame.png")
    , m_isLoadState(load)
    , m_enabled(false)
{
    m_title = std::make_shared<sf::Sprite>(*m_titleTexture);
    m_title->setPosition(-m_title->getGlobalBounds().width/2.0f, -m_title->getGlobalBounds().height-120);

    m_returnButton = std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(-30, -10), sf::Vector2f(100, 25), Margin(1), VAlign::V_BOTTOM, HAlign::H_RIGHT), tr("Cancel"));
    m_returnButton->connectClickEvent(std::bind(&onReturn, this));

    createItemsList();
    if(!m_items.empty())
        m_items.front().playButton->changeActiveState(Widget::ControlState::ACTIVE);
    else if(load)
        m_returnButton->changeActiveState(Widget::ControlState::ACTIVE);
    else m_newSave.saveName->changeActiveState(Widget::ControlState::ACTIVE);
}

void LoadSaveState::createItemsList()
{
    m_items.clear();

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
                                                                        , sf::Vector2f(m_frame->getSize().x-2*OffsetText, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER)
                                                        , m_isLoadState ? tr("Play") : tr("Save"));
        item.removeButton = std::make_shared<BasicButton>(AdaptableBounds(sf::Vector2f(pos.x+m_frame->getSize().x/2, pos.y+OffsetText+2*ButtonOffset)
                                                                          , sf::Vector2f(m_frame->getSize().x-2*OffsetText, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Delete"));

        item.playButton->connectClickEvent(std::bind(m_isLoadState ? &onPressPlayButton : &onPressSaveButton, this, file));
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
        if(i+2 < m_items.size())
            item.removeButton->connect(MOVE_DOWN, m_items[i+2].playButton);
    }

    if(m_isLoadState)
    {
        if(!m_items.empty())
        {
            m_items.back().removeButton->connect(MOVE_DOWN, m_returnButton);
            m_returnButton->connect(MOVE_UP, m_items.back().removeButton);
        }
        if(m_items.size() > 1)
            m_items[m_items.size()-2].removeButton->connect(MOVE_DOWN, m_returnButton);
    }
    else
    {
        m_newSave.useSave = true;
        auto index = m_items.size();
        sf::Vector2f pos(index%2 == 0 ? -HFrameOffset-m_frame->getSize().x : HFrameOffset, (index/2)*(VFrameOffset+m_frame->getSize().y)+TopFrame);
        m_newSave.frame = std::make_shared<sf::Sprite>(*m_frame);
        m_newSave.frame->setPosition(pos);

        m_newSave.saveName = std::make_shared<BasicTextBox>(AdaptableBounds(sf::Vector2f(pos.x+m_frame->getSize().x/2, pos.y+OffsetText+0.5f*ButtonOffset)
                                                                        , sf::Vector2f(m_frame->getSize().x-2*OffsetText, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER));
        m_newSave.saveButton = std::make_shared<BasicButton>(AdaptableBounds(sf::Vector2f(pos.x+m_frame->getSize().x/2, pos.y+OffsetText+1.5f*ButtonOffset)
                                                                          , sf::Vector2f(m_frame->getSize().x-2*OffsetText, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("New"));

        m_newSave.saveButton->connectClickEvent(std::bind(&onPressNewButton, this));
        m_newSave.saveName->connect(MOVE_DOWN, m_newSave.saveButton);
        m_newSave.saveButton->connect(MOVE_UP, m_newSave.saveName);
        m_newSave.saveButton->connect(MOVE_DOWN, m_returnButton);
        m_returnButton->connect(MOVE_UP, m_newSave.saveButton);

        if(!m_items.empty())
        {
            m_items.back().removeButton->connect(MOVE_DOWN, m_returnButton);
            if(m_items.size()%2 != 0)
            {
                m_newSave.saveButton->connect(MOVE_LEFT, m_items.back().playButton);
                m_items.back().playButton->connect(MOVE_RIGHT, m_newSave.saveButton);
                m_items.back().removeButton->connect(MOVE_RIGHT, m_newSave.saveButton);
            }
            if(m_items.size() > 1)
            {
                m_items[m_items.size()-2].removeButton->connect(MOVE_DOWN,  m_newSave.saveName);
                m_newSave.saveName->connect(MOVE_UP, m_items[m_items.size()-2].removeButton);
            }
        }
    }
}

void LoadSaveState::enable()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
        m->setClearColor(sf::Color::White);

    Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(sf::Vector2f(0, 0)));

    DrawableList::add(m_title, DrawableList::DrawHeight::MENU + 2);

    DrawableList::add(m_returnButton, DrawableList::DrawHeight::MENU);
    Updatable::add(m_returnButton);
    Controlable::add(m_returnButton);

    for(const auto & item : m_items)
    {
        DrawableList::add(item.frame, DrawableList::DrawHeight::MENU);
        DrawableList::add(item.saveName, DrawableList::DrawHeight::MENU + 1);

        DrawableList::add(item.playButton, DrawableList::DrawHeight::MENU + 1);
        Updatable::add(item.playButton);
        Controlable::add(item.playButton);

        DrawableList::add(item.removeButton, DrawableList::DrawHeight::MENU + 1);
        Updatable::add(item.removeButton);
        Controlable::add(item.removeButton);
    }

    if(m_newSave.useSave)
    {
        DrawableList::add(m_newSave.frame, DrawableList::DrawHeight::MENU);

        DrawableList::add(m_newSave.saveName, DrawableList::DrawHeight::MENU + 1);
        Updatable::add(m_newSave.saveName);
        Controlable::add(m_newSave.saveName);

        DrawableList::add(m_newSave.saveButton, DrawableList::DrawHeight::MENU + 1);
        Updatable::add(m_newSave.saveButton);
        Controlable::add(m_newSave.saveButton);
    }

    m_enabled = true;
}

void LoadSaveState::disable()
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

    if(m_newSave.useSave)
    {
        DrawableList::del(m_newSave.frame);

        DrawableList::del(m_newSave.saveName);
        Updatable::del(m_newSave.saveName);
        Controlable::del(m_newSave.saveName);

        DrawableList::del(m_newSave.saveButton);
        Updatable::del(m_newSave.saveButton);
        Controlable::del(m_newSave.saveButton);
    }

    m_enabled = false;
}

void LoadSaveState::onPressPlayButton(std::string file)
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        if(m->isCurrentSubstate())
            m->delSubstate();
        std::unique_ptr<State> s(std::make_unique<GameState>(m_machine, file));
        m->setNext(s);
    }
}

void LoadSaveState::onPressSaveButton(std::string file)
{
    save(file);
    onReturn();
}

void LoadSaveState::onPressRemoveButton(std::string file)
{
    remove(file);
    createItemsList();
    if(m_returnButton->getState() == Controlable::ControlState::UNACTIVE)
    {
        if(!m_items.empty())
            m_items.front().playButton->changeActiveState(Widget::ControlState::ACTIVE);
        else if(m_newSave.useSave)
            m_newSave.saveButton->changeActiveState(Widget::ControlState::ACTIVE);
        else m_returnButton->changeActiveState(Widget::ControlState::ACTIVE);
    }

    if(m_enabled)
        enable();
}

void LoadSaveState::onPressNewButton()
{
    if(!m_newSave.useSave)
        return;
    if(!m_newSave.saveName)
        return;
    std::string filename(m_newSave.saveName->text);
    if(!isValidFilename(filename))
        return;
    filename = saveDir + filename + ".json";
    save(filename);
    onReturn();
}

void LoadSaveState::onReturn()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        if(m->isCurrentSubstate())
            m->delSubstate();
        else
        {
            std::unique_ptr<State> s(std::make_unique<MenuState>(m_machine));
            m->setNext(s);
        }
    }
}
