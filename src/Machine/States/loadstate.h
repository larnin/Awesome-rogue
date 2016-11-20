#ifndef LOADSTATE_H
#define LOADSTATE_H

#include "state.h"
#include "Utilities/ressource.h"
#include "GUI/Widgets/Buttons/basicbutton.h"
#include "GUI/Widgets/label.h"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class LoadState : public State
{
public:
    LoadState(std::weak_ptr<StateMachine> machine);
    LoadState(LoadState &&) = default;
    LoadState & operator =(LoadState &&) = default;
    virtual ~LoadState() = default;

protected:
    virtual void enable();
    virtual void disable();

private:
    void createItemsList();

    void onPressPlayButton(std::string file);
    void onPressRemoveButton(std::string file);
    void onReturn();

    struct LoadItem
    {
        std::shared_ptr<BasicButton> playButton;
        std::shared_ptr<BasicButton> removeButton;
        std::shared_ptr<Label> saveName;
        std::shared_ptr<sf::Sprite> frame;
    };

    std::shared_ptr<sf::Sprite> m_title;
    Texture m_titleTexture;
    Texture m_frame;
    std::vector<LoadItem> m_items;
    std::shared_ptr<BasicButton> m_returnButton;

};

#endif // LOADSTATE_H
