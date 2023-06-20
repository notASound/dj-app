#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

class MainComponent : public juce::AudioAppComponent,
                      public juce::Button::Listener,
                      public juce::Slider::Listener
{
public:
    /** constructor and deconstructor **/
    MainComponent();
    ~MainComponent() override;

    /** implement pure virtual functions from AudioAppComponent class that requires implementation in inheritance class MainComponent */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    /** implement functions responsible for painting and resizing app elements */
    void paint(juce::Graphics &g) override;
    void resized() override;
    
    /** implement Button::Listener*/
    void buttonClicked(juce::Button *)override;
    
    /** implement Slider::Listener*/
    void sliderValueChanged(juce::Slider* slider) override;
    
private:
    juce::Random random;
    bool playing;
    double gain;

    /** create AudioFormatManager */
    juce::AudioFormatManager formatManager;
    
    /** create AudioThumbnailCache */
    juce::AudioThumbnailCache thumbCache{100};

    /** create FileChooser */
    juce::FileChooser fChooser{"Select a file..."};

    /** create instance of DJAudioPlayer class */
    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};

    /** create instance of DeckGUI class */
    DeckGUI deck1{&player1, formatManager, thumbCache};
    DeckGUI deck2{&player2, formatManager, thumbCache};
    
    /** create track meta data */
    DJAudioPlayer metaData{formatManager};

    /** add a mixer */
    juce::MixerAudioSource mixerSource;
  
    /** add a playlist component */
    PlaylistComponent playlistComponent{&deck1, &deck2, &metaData};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
