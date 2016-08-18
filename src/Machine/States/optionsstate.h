#ifndef OPTIONSSTATE_H
#define OPTIONSSTATE_H

#include "state.h"
#include <vector>
#include <memory>

#include "Controles/keyconfig.h"

class KeyConfigWidget;
class Widget;
class BasicButton;
class BasicSlider;
template <typename T>
class RadioButtons;
class BasicRadioButtonItem;
class Label;
class SimpleControler;
class BasicCheckbox;

class OptionsState : public State
{
public:
    OptionsState(std::weak_ptr<StateMachine> machine, StateType oldState);
    OptionsState(OptionsState &&) = default;
    OptionsState & operator =(OptionsState &&) = default;
    virtual ~OptionsState() = default;


    virtual void enable();
    virtual void disable();

private:
    void initPG1();
    void initPG2();
    void jumpToPG1();
    void jumpTOPG2();
    void disablePages();

    void onValidClicked();
    void onAnnuleClicked();
    void onKeyModified(CommandType, KeyInfo);
    void onZoomChanged(std::string zoomName);
    void onMusicVolumeChanged(float value);
    void onSoundVolumeChanged(float value);
    void onFullScreenToggle(bool value);

    KeysConfig generateCurrentKeyconfig() const;
    void exitState();

    StateType m_oldState;
    std::shared_ptr<SimpleControler> m_pageControler;
    unsigned int m_pageID;
    std::shared_ptr<Label> m_stateName;
    std::shared_ptr<BasicButton> m_validButton;
    std::shared_ptr<BasicButton> m_annuleButton;

    // keyconfig page
    KeysConfig m_originalKeyConfig;
    std::vector<std::shared_ptr<KeyConfigWidget>> m_keys;
    std::vector<std::shared_ptr<Label>> m_keysLabels;

    // other configs page
    std::vector<std::shared_ptr<Label>> m_othersLabels;
    std::shared_ptr<RadioButtons<BasicRadioButtonItem>> m_windowZoom;
    std::shared_ptr<BasicCheckbox> m_fullScreen;
    std::shared_ptr<BasicSlider> m_musicVolume;
    std::shared_ptr<BasicSlider> m_soundsVolume;
};

#endif // OPTIONSSTATE_H