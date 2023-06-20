#pragma once
#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer *_djAudioPlayer,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    
    ~DeckGUI() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(juce::Button *button) override;

    /** implement Slider::Listener */
    void sliderValueChanged(juce::Slider *slider) override;

    /** implement drag&drop functionality */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    void timerCallback() override;
    
    /** keyboard shortcuts */
    bool keyPressed(const juce::KeyPress &k) override;
    
    /** load a file into the deck */
    void loadFile(juce::URL URL);
    void load();
    
    /** rewind the track -15 sec */
    void rewind();
    
    /** fast forward the track +15 sec */
    void forward();
    
private:
    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"PAUSE"};
    juce::TextButton loadButton{"LOAD"};
    juce::TextButton cueButton{"CUE"};
    juce::TextButton forwardButton{">>"};
    juce::TextButton rewindButton{"<<"};
    
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Slider speedSlider;
    
    juce::Label volumeLabel;
    juce::Label positionLabel;
    juce::Label speedLabel;
    
    juce::FileChooser fChooser{"Select a file..."};

    DJAudioPlayer *djAudioPlayer;
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
