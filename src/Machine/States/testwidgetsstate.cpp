#include "testwidgetsstate.h"
#include "GUI/Widgets/graphichelper.h"
#include "Systemes/drawablelist.h"

#include "GUI/Widgets/Buttons/basicbutton.h"
#include "GUI/Widgets/Sliders/basicslider.h"
#include "GUI/Widgets/Togglable/togglablebasicbutton.h"
#include "GUI/Widgets/TextBoxs/basictextbox.h"
#include "GUI/Widgets/Togglable/basiccheckbox.h"
#include "GUI/Widgets/ComboBoxs/basiccombobox.h"
#include "GUI/Widgets/Sliders/basicquantifiedslider.h"
#include "GUI/Widgets/RadioButtons/radiobuttons.h"
#include "GUI/Widgets/RadioButtons/basicradiobuttonitem.h"

TestWidgetsState::TestWidgetsState(std::weak_ptr<StateMachine> machine)
    : State(machine)
{
    std::shared_ptr<BasicButton> w = std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, 0), sf::Vector2f(100, 25), Margin(1), VAlign::V_TOP, HAlign::H_CENTER), "Button");
    w->changeActiveState(Widget::ControlState::ACTIVE);
    add(w);

    std::shared_ptr<TogglableBasicButton> w2 = std::make_shared<TogglableBasicButton>
            (AdaptableBounds(sf::Vector2f(0, 30), sf::Vector2f(90, 25), Margin(1), VAlign::V_TOP, HAlign::H_CENTER), "Togglable");
    add(w2);

    std::shared_ptr<BasicSlider> w3 = std::make_shared<BasicSlider>
            (AdaptableBounds(sf::Vector2f(0, 60), sf::Vector2f(150, 25), Margin(1), VAlign::V_TOP, HAlign::H_CENTER));
    w3->min = 0;
    w3->max = 100;
    w3->speed = 30.0f;
    add(w3);

    std::shared_ptr<BasicTextBox> w4 = std::make_shared<BasicTextBox>
            (AdaptableBounds(sf::Vector2f(0, 90), sf::Vector2f(150, 25), Margin(1), VAlign::V_TOP, HAlign::H_CENTER));
    add(w4);

    std::shared_ptr<BasicCheckbox> w5 = std::make_shared<BasicCheckbox>
            (AdaptableBounds(sf::Vector2f(0, 120), sf::Vector2f(120, 25), Margin(1), VAlign::V_TOP, HAlign::H_CENTER), "Use that !");
    add(w5);

    std::shared_ptr<BasicCombobox> w6 = std::make_shared<BasicCombobox>
            (AdaptableBounds(sf::Vector2f(0, 150), sf::Vector2f(90, 25), Margin(1), VAlign::V_TOP, HAlign::H_CENTER));
    w6->addItem("Obj 1");
    w6->addItem("Obj 2");
    w6->addItem("Obj 3");
    w6->addItem("Obj 4");
    add(w6, 2);

    std::shared_ptr<BasicQuantifiedSlider> w7 = std::make_shared<BasicQuantifiedSlider>
            (AdaptableBounds(sf::Vector2f(0, 180), sf::Vector2f(120, 35), Margin(1), VAlign::V_TOP, HAlign::H_CENTER));
    w7->addItem("Low");
    w7->addItem("Med.");
    w7->addItem("Hight");
    add(w7);

    std::shared_ptr<RadioButtons<BasicRadioButtonItem>> w8 = std::make_shared<RadioButtons<BasicRadioButtonItem>>
            (AdaptableBounds(sf::Vector2f(0, 220), sf::Vector2f(80, 63), Margin(1), VAlign::V_TOP, HAlign::H_CENTER));
    w8->addButton("Radio 1");
    w8->addButton("Radio 2");
    w8->addButton("Radio 3");
    add(w8);

    w->connect(CommandType::MOVE_DOWN, w2);
    w2->connect(CommandType::MOVE_UP, w);
    w2->connect(CommandType::MOVE_DOWN, w3);
    w3->connect(CommandType::MOVE_UP, w2);
    w3->connect(CommandType::MOVE_DOWN, w4);
    w4->connect(CommandType::MOVE_UP, w3);
    w4->connect(CommandType::MOVE_DOWN, w5);
    w5->connect(CommandType::MOVE_UP, w4);
    w5->connect(CommandType::MOVE_DOWN, w6);
    w6->connect(CommandType::MOVE_UP, w5);
    w6->connect(CommandType::MOVE_DOWN, w7);
    w7->connect(CommandType::MOVE_UP, w6);
    w7->connect(CommandType::MOVE_DOWN, w8);
    w8->connect(CommandType::MOVE_UP, w7);
}

void TestWidgetsState::add(std::shared_ptr<Widget> w, unsigned int height)
{
    DrawableList::add(w, height);
    Controlable::add(w);
    Updatable::add(w);
    m_widgets.push_back(w);
}
