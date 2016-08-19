#include "optionsstate.h"
#include "GUI/Widgets/keyconfigwidget.h"
#include "GUI/Widgets/label.h"
#include "GUI/Widgets/Buttons/basicbutton.h"
#include "Machine/statemachine.h"
#include "Events/event.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "GUI/Widgets/Buttons/basicbutton.h"
#include "GUI/Widgets/RadioButtons/radiobuttons.h"
#include "GUI/Widgets/RadioButtons/basicradiobuttonitem.h"
#include "GUI/Widgets/Sliders/basicslider.h"
#include "GUI/Widgets/Togglable/basiccheckbox.h"
#include "GUI/simplecontroler.h"
#include "Systemes/drawablelist.h"
#include "Utilities/configs.h"

OptionsState::OptionsState(std::weak_ptr<StateMachine> machine)
    : State(machine)
    , m_pageID(0)
    , m_originalKeyConfig(KeysConfig::keysFilename)
{
    std::shared_ptr<StateMachine> m(machine.lock());
     if(!m)
         return;
    Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(sf::Vector2f(0, 0)));

    Font f("res/font/PressStart2P.ttf");

    m_pageControler = std::make_shared<SimpleControler>();
    m_pageControler->set(CommandType::KEY_LEFT_PAGE, std::bind(&jumpToPG1, this));
    m_pageControler->set(CommandType::KEY_RIGHT_PAGE, std::bind(&jumpTOPG2, this));

    m_stateName = std::make_shared<Label>(AdaptableBounds(sf::Vector2f(0, 15), sf::Vector2f(250, 30), Margin(0), VAlign::V_TOP, HAlign::H_CENTER));
    m_stateName->setText("Options", f, 16, sf::Color::Black);

    m_validButton = std::make_shared<BasicButton>(AdaptableBounds(sf::Vector2f(-110, -5), sf::Vector2f(100, 25), Margin(1)
                                                                  , VAlign::V_BOTTOM, HAlign::H_RIGHT), "Valider");
    m_annuleButton = std::make_shared<BasicButton>(AdaptableBounds(sf::Vector2f(-5, -5), sf::Vector2f(100, 25), Margin(1)
                                                                   , VAlign::V_BOTTOM, HAlign::H_RIGHT), "Annuler");

    m_validButton->connect(MOVE_RIGHT, m_annuleButton);
    m_annuleButton->connect(MOVE_LEFT, m_validButton);

    m_validButton->connectClickEvent(std::bind(&onValidClicked, this));
    m_annuleButton->connectClickEvent(std::bind(&onAnnuleClicked, this));

    initPG1();
    initPG2();
}

void OptionsState::initPG1()
{
    Font f("res/font/PressStart2P.ttf");
    const float topOffset(60.0f);
    const float vOffset(30.0f);
    const float hOffset(150.0f);
    const float labelOffset(50.0f);
    KeysConfig keys(m_originalKeyConfig);

    for(unsigned int i(0) ; i < COMMANDS_COUNT ; i++)
    {
        bool isLeft(i%2 == 0);
        std::shared_ptr<Label> l(std::make_shared<Label>(AdaptableBounds(sf::Vector2f(hOffset*(isLeft ? -1 : 1) - labelOffset ,topOffset+vOffset*(i/2))
                                                                         , sf::Vector2f(100, vOffset-5.0f), Margin(0), V_TOP, H_CENTER)));
        l->setText(commandTypeToString(CommandType(i)) + " :", f, 8, sf::Color::Black);
        m_keysLabels.push_back(l);

        std::shared_ptr<KeyConfigWidget> k(std::make_shared<KeyConfigWidget>(CommandType(i), keys.commands[i]
                                                                           , AdaptableBounds(sf::Vector2f(hOffset*(isLeft ? -1 : 1) + labelOffset ,topOffset+vOffset*(i/2)-10)
                                                                                           , sf::Vector2f(100, vOffset - 5.0f), Margin(0), V_TOP, H_CENTER)));
        k->connectKeyChanged(std::bind(&onKeyModified, this, _1, _2));
        m_keys.push_back(k);
    }

    for(unsigned int i(0) ; i < COMMANDS_COUNT ; i++)
    {
        if(i > 1)
            m_keys[i]->connect(MOVE_UP, m_keys[i-2]);
        if(i < COMMANDS_COUNT - 2)
            m_keys[i]->connect(MOVE_DOWN, m_keys[i+2]);
        if(i%2 == 0 && i < COMMANDS_COUNT - 1)
            m_keys[i]->connect(MOVE_RIGHT, m_keys[i+1]);
        if(i%2 != 0 && i > 0)
            m_keys[i]->connect(MOVE_LEFT, m_keys[i-1]);
    }

    m_keys[COMMANDS_COUNT-1]->connect(MOVE_DOWN, m_validButton);
    m_keys[COMMANDS_COUNT-2]->connect(MOVE_DOWN, m_validButton);
}

void OptionsState::initPG2()
{
    Font f("res/font/PressStart2P.ttf");
    const float topOffset(60.0f);
    const float vOffset(30.0f);
    const float labelOffset(10.0f);

    m_othersLabels.push_back(std::make_shared<Label>(AdaptableBounds(sf::Vector2f(-100, topOffset), sf::Vector2f(150, vOffset-5), Margin(0), V_TOP, H_CENTER)));
    m_othersLabels.back()->setText("Zoom :", f, 8, sf::Color::Black);
    m_windowZoom = std::make_shared<RadioButtons<BasicRadioButtonItem>>(AdaptableBounds(sf::Vector2f(100, topOffset - labelOffset), sf::Vector2f(150, vOffset-5), Margin(0), V_TOP, H_CENTER), false);
    m_windowZoom->addButton("*1");
    m_windowZoom->addButton("*2");
    m_windowZoom->addButton("*3");
    m_windowZoom->setCurrent(Configs::c.zoom < 0.4f ? "*3" : Configs::c.zoom < 0.75f ? "*2" : "*1");
    m_windowZoom->connectIndexChangedEvent(std::bind(&onZoomChanged, this, _1));

    m_othersLabels.push_back(std::make_shared<Label>(AdaptableBounds(sf::Vector2f(-100, topOffset + vOffset), sf::Vector2f(150, vOffset-5), Margin(0), V_TOP, H_CENTER)));
    m_othersLabels.back()->setText("Plein écran :", f, 8, sf::Color::Black);
    m_fullScreen = std::make_shared<BasicCheckbox>(AdaptableBounds(sf::Vector2f(100, topOffset + vOffset - labelOffset), sf::Vector2f(150, vOffset-5), Margin(0), V_TOP, H_CENTER));
    m_fullScreen->state = Configs::c.useFullScreen;
    m_fullScreen->connecToggledEvent(std::bind(&onFullScreenToggle, this, _1));

    m_othersLabels.push_back(std::make_shared<Label>(AdaptableBounds(sf::Vector2f(-100, topOffset + 2*vOffset), sf::Vector2f(150, vOffset-5), Margin(0), V_TOP, H_CENTER)));
    m_othersLabels.back()->setText("Music :", f, 8, sf::Color::Black);

    m_musicVolume = std::make_shared<BasicSlider>(AdaptableBounds(sf::Vector2f(100, topOffset + 2*vOffset - labelOffset), sf::Vector2f(150, vOffset-5), Margin(0), V_TOP, H_CENTER));
    m_musicVolume->min = 0;
    m_musicVolume->max = 100;
    m_musicVolume->value = Configs::c.musicVolum;
    m_musicVolume->speed = 25;
    m_musicVolume->connectValueChangedEvent(std::bind(&onMusicVolumeChanged, this, 1));

    m_othersLabels.push_back(std::make_shared<Label>(AdaptableBounds(sf::Vector2f(-100, topOffset + 3*vOffset), sf::Vector2f(150, vOffset-5), Margin(0), V_TOP, H_CENTER)));
    m_othersLabels.back()->setText("Sounds :", f, 8, sf::Color::Black);

    m_soundsVolume = std::make_shared<BasicSlider>(AdaptableBounds(sf::Vector2f(100, topOffset + 3*vOffset - labelOffset), sf::Vector2f(150, vOffset-5), Margin(0), V_TOP, H_CENTER));
    m_soundsVolume->min = 0;
    m_soundsVolume->max = 100;
    m_soundsVolume->value = Configs::c.soundsVolum;
    m_soundsVolume->speed = 25;
    m_soundsVolume->connectValueChangedEvent(std::bind(&onSoundVolumeChanged, this, _1));

    m_windowZoom->connect(MOVE_DOWN, m_fullScreen);
    m_fullScreen->connect(MOVE_UP, m_windowZoom);
    m_fullScreen->connect(MOVE_DOWN, m_musicVolume);
    m_musicVolume->connect(MOVE_UP, m_fullScreen);
    m_musicVolume->connect(MOVE_DOWN, m_soundsVolume);
    m_soundsVolume->connect(MOVE_UP, m_musicVolume);
    m_soundsVolume->connect(MOVE_DOWN, m_validButton);
}


void OptionsState::enable()
{
    DrawableList::add(m_stateName, 1);
    DrawableList::add(m_validButton, 2);
    Updatable::add(m_validButton);
    Controlable::add(m_validButton);
    DrawableList::add(m_annuleButton, 2);
    Updatable::add(m_annuleButton);
    Controlable::add(m_annuleButton);
    Controlable::add(m_pageControler);

    if(m_pageID == 0)
        jumpToPG1();
    else jumpTOPG2();
}

void OptionsState::disable()
{
    disablePages();

    DrawableList::del(m_stateName);
    DrawableList::del(m_validButton);
    Updatable::del(m_validButton);
    Controlable::del(m_validButton);
    DrawableList::del(m_annuleButton);
    Updatable::del(m_annuleButton);
    Controlable::del(m_annuleButton);
    Controlable::del(m_pageControler);
}

void OptionsState::jumpToPG1()
{
    disablePages();

    for(const auto & l : m_keysLabels)
        DrawableList::add(l, 0);
    for(const auto & k : m_keys)
    {
        DrawableList::add(k, 0);
        Updatable::add(k);
        Controlable::add(k);
    }

    m_keys.front()->changeActiveState(Controlable::ACTIVE);

    m_validButton->connect(MOVE_UP, m_keys.back());
    m_annuleButton->connect(MOVE_UP, m_keys.back());

    m_pageID = 0;
}

void OptionsState::jumpTOPG2()
{
    disablePages();

    for(const auto & l : m_othersLabels)
        DrawableList::add(l, 0);

    DrawableList::add(m_windowZoom, 0);
    Updatable::add(m_windowZoom);
    Controlable::add(m_windowZoom);

    DrawableList::add(m_fullScreen, 0);
    Updatable::add(m_fullScreen);
    Controlable::add(m_fullScreen);

    DrawableList::add(m_musicVolume, 0);
    Updatable::add(m_musicVolume);
    Controlable::add(m_musicVolume);

    DrawableList::add(m_soundsVolume, 0);
    Updatable::add(m_soundsVolume);
    Controlable::add(m_soundsVolume);

    m_windowZoom->changeActiveState(Controlable::ACTIVE);

    m_validButton->connect(MOVE_UP, m_soundsVolume);
    m_annuleButton->connect(MOVE_UP, m_soundsVolume);

    m_pageID = 1;
}

void OptionsState::disablePages()
{
    //disable page 1
    for(auto & l : m_keysLabels)
    {
        DrawableList::del(l);
    }
    for(const auto & k : m_keys)
    {
        DrawableList::del(k);
        Updatable::del(k);
        Controlable::del(k);
        k->changeActiveState(Controlable::UNACTIVE);
    }

    //disable page 2
    for(const auto & l : m_othersLabels)
        DrawableList::del(l);

    DrawableList::del(m_windowZoom);
    Updatable::del(m_windowZoom);
    Controlable::del(m_windowZoom);
    m_windowZoom->changeActiveState(Controlable::UNACTIVE);

    DrawableList::del(m_fullScreen);
    Updatable::del(m_fullScreen);
    Controlable::del(m_fullScreen);
    m_fullScreen->changeActiveState(Controlable::UNACTIVE);

    DrawableList::del(m_musicVolume);
    Updatable::del(m_musicVolume);
    Controlable::add(m_musicVolume);
    m_musicVolume->changeActiveState(Controlable::UNACTIVE);

    DrawableList::del(m_soundsVolume);
    Updatable::del(m_soundsVolume);
    Controlable::del(m_soundsVolume);
    m_soundsVolume->changeActiveState(Controlable::UNACTIVE);

    m_validButton->disconnect(MOVE_UP);
    m_validButton->changeActiveState(Controlable::UNACTIVE);
    m_annuleButton->disconnect(MOVE_UP);
    m_annuleButton->changeActiveState(Controlable::UNACTIVE);
}

void OptionsState::onValidClicked()
{
    generateCurrentKeyconfig().save(KeysConfig::keysFilename);

    Configs::c.useFullScreen = m_fullScreen->state;
    Configs::c.zoom = m_windowZoom->getCurrent() == "*1" ? 1.0f : m_windowZoom->getCurrent() == "*2" ? 0.5f : 0.33f;
    Configs::c.musicVolum = m_musicVolume->value;
    Configs::c.soundsVolum = m_soundsVolume->value;

    exitState();
}

void OptionsState::onAnnuleClicked()
{
    auto m(m_machine.lock());
    if(!m)
        return;
    m->setKeys(m_originalKeyConfig);

    if(Configs::c.useFullScreen != m_fullScreen->state)
        m->changeFullScreen(Configs::c.useFullScreen);
    m->changeWindowZoom(m_windowZoom->getCurrent() == "*1" ? 1.0f : m_windowZoom->getCurrent() == "*2" ? 0.5f : 0.33f);

    //add music and sounds !!!

    exitState();
}

void OptionsState::exitState()
{
    auto m(m_machine.lock());
    if(!m)
        return;
    m->delSubstate();
}

void OptionsState::onKeyModified(CommandType, KeyInfo)
{
    auto m(m_machine.lock());
    if(!m)
        return;

    m->setKeys(generateCurrentKeyconfig());
}

void OptionsState::onZoomChanged(std::string zoomName)
{
    auto m(m_machine.lock());
    if(m)
    {
        float zoom(zoomName == "*1" ? 1.0f : zoomName == "*2" ? 0.5f : 0.33f);
        m->changeWindowZoom(zoom);
    }
}

void OptionsState::onMusicVolumeChanged(float value)
{
    //todo
}

void OptionsState::onSoundVolumeChanged(float value)
{
    //todo
}

void OptionsState::onFullScreenToggle(bool value)
{
    auto m(m_machine.lock());
    if(m)
        m->changeFullScreen(value);
}

KeysConfig OptionsState::generateCurrentKeyconfig() const
{
    KeysConfig k;
    for(unsigned int i(0) ; i < COMMANDS_COUNT ; i++)
        k.commands[i] = m_keys[i]->getKey();
    int joy(getFirstConnectedJoystic());
    if(joy >= 0)
    {
        k.useJoystic = true;
        k.joysticID = joy;
    }
    else k.useJoystic = false;

    return k;
}
