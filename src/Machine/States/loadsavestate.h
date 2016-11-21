#ifndef LOADSTATE_H
#define LOADSTATE_H

#include "state.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class BasicButton;
class BasicTextBox;
class Label;

class LoadSaveState : public State
{
public:
    LoadSaveState(std::weak_ptr<StateMachine> machine, bool load);
    LoadSaveState(LoadSaveState &&) = default;
    LoadSaveState & operator =(LoadSaveState &&) = default;
    virtual ~LoadSaveState() = default;

protected:
    virtual void enable();
    virtual void disable();

private:
    void createItemsList();

    void onPressPlayButton(std::string file);
    void onPressSaveButton(std::string file);
    void onPressNewButton();
    void onPressRemoveButton(std::string file);
    void onReturn();

    struct LoadItem
    {
        std::shared_ptr<BasicButton> playButton;
        std::shared_ptr<BasicButton> removeButton;
        std::shared_ptr<Label> saveName;
        std::shared_ptr<sf::Sprite> frame;
    };

    struct NewSaveItem
    {
        bool useSave = false;
        std::shared_ptr<BasicTextBox> saveName;
        std::shared_ptr<BasicButton> saveButton;
        std::shared_ptr<sf::Sprite> frame;
    };

    std::shared_ptr<sf::Sprite> m_title;
    Texture m_titleTexture;
    Texture m_frame;
    std::vector<LoadItem> m_items;
    NewSaveItem m_newSave;
    std::shared_ptr<BasicButton> m_returnButton;
    bool m_isLoadState;

};

#endif // LOADSTATE_H
