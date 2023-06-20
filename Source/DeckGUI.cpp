#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer *_djAudioPlayer,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse
            ) : djAudioPlayer{_djAudioPlayer},
            waveformDisplay(formatManagerToUse, cacheToUse)
{
    /** make buttons, waveform and sliders visible */
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(cueButton);
    addAndMakeVisible(forwardButton);
    addAndMakeVisible(rewindButton);

    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(speedSlider);
                
    addAndMakeVisible(waveformDisplay);
                
    /** add labels to sliders */
    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(positionLabel);
    addAndMakeVisible(speedLabel);
    
    /** slider label settings */
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
                                false,
                                 40,
                                 volumeSlider.getTextBoxHeight());
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.attachToComponent(&volumeSlider, true);

    positionSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
                                false,
                                 40,
                                 positionSlider.getTextBoxHeight());
    positionLabel.setText("Position", juce::dontSendNotification);
    positionLabel.attachToComponent(&positionSlider, true);
                
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove,
                                false,
                                 40,
                                 speedSlider.getTextBoxHeight());
    speedLabel.setText("Pitch", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);
                
    /** add listeners to buttons and sliders */
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    cueButton.addListener(this);
    forwardButton.addListener(this);
    rewindButton.addListener(this);

    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);
    
    /** set sliders range */
    volumeSlider.setRange(0, 1);
    speedSlider.setRange(0.9, 1.1); // Limits possible track speed change (pitch) to +-10%
    positionSlider.setRange(0, 1);
                
    /** set sliders number decimal */
    volumeSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    positionSlider.setNumDecimalPlacesToDisplay(2);
                
    /** start timer */
    startTimer(200);

    /** change slider styles  */
        volumeSlider.setSliderStyle(juce::Slider::Rotary);
        speedSlider.setSliderStyle(juce::Slider::LinearVertical);
}

DeckGUI::~DeckGUI()
{
    /** stop timer */
    stopTimer();
}

void DeckGUI::paint(juce::Graphics &g)
{
    playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    playButton.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
    stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    stopButton.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
    cueButton.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    cueButton.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
    loadButton.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    loadButton.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
    forwardButton.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    forwardButton.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
    rewindButton.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    rewindButton.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
}

void DeckGUI::resized()
{
    float rowH = getHeight()/ 8;
    float columnW = getWidth() / 8;
    
    waveformDisplay.setBounds(columnW * 2, 0, getWidth(), rowH * 2);
    
    speedSlider.setBounds(columnW, 0, columnW, rowH * 2);
    positionSlider.setBounds(columnW, rowH * 2, getWidth() , rowH);
    
    playButton.setBounds(0, rowH * 4, columnW * 2, rowH);
    stopButton.setBounds(columnW * 2, rowH * 4, columnW * 2, rowH);
    volumeSlider.setBounds(columnW * 6, rowH * 4, columnW * 2, rowH * 2);
    cueButton.setBounds(0, rowH * 5, columnW * 2, rowH);
    loadButton.setBounds(columnW * 2, rowH * 5, columnW * 2, rowH);
    rewindButton.setBounds(columnW * 2, rowH * 3, columnW * 3, rowH);
    forwardButton.setBounds(columnW * 5, rowH * 3, columnW * 3, rowH);
}

/** handle buttons clicks*/
void DeckGUI::buttonClicked(juce::Button *button)
{
    if (button == &playButton)
    {
        djAudioPlayer->start();
    };

    if (button == &stopButton)
    {
        djAudioPlayer->stop();
    };

    if (button == &loadButton)
    {
        load();
    };
    if (button == &cueButton)
    {
        djAudioPlayer->cue();
    }
    
    if (button == &rewindButton)
    {
        rewind();
    }
    
    if (button == &forwardButton)
    {
        forward();
    }
};

bool DeckGUI::keyPressed(const juce::KeyPress &k)
{
    if(k.getTextCharacter() == 'p') {
        djAudioPlayer->start();
    }
    
    if(k.getTextCharacter() == 's') {
        djAudioPlayer->stop();
    }
    
    if(k.getTextCharacter() == '-') {
        rewind();
    }
    
    if(k.getTextCharacter() == '+') {
        forward();
    }
    
    if(k.getTextCharacter() == 'l') {
        load();
    }
    
    if(k.getTextCharacter() == 'c') {
        djAudioPlayer->cue();
    }
};

/** handle value change on sliders */
void DeckGUI::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &volumeSlider)
    {
        djAudioPlayer->setGain(slider->getValue());
    };
    if (slider == &speedSlider)
    {
        djAudioPlayer->setSpeed(slider->getValue());
    };
    if (slider == &positionSlider)
    {
        djAudioPlayer->setPositionRelative(slider->getValue());
    };
};

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}
void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y)
{

    for (juce::String filename : files)
    {
        juce::URL fileURL = juce::URL{juce::File{filename}}; // converts the String to a File and the File to an URL
        djAudioPlayer->loadURL(fileURL);
        waveformDisplay.loadURL(fileURL);
        return;
    }
};

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
        djAudioPlayer->getPositionRelative());
};

void DeckGUI::loadFile(juce::URL URL)
{
    djAudioPlayer->loadURL(URL);
    waveformDisplay.loadURL(URL);
};

void DeckGUI::rewind()
{
    positionSlider.setValue(djAudioPlayer->getPositionRelative() - 0.015);
};

void DeckGUI::forward()
{
    positionSlider.setValue(djAudioPlayer->getPositionRelative() + 0.015);
};

void DeckGUI::load()
{
    auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectFiles;

    fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser &chooser)
                         {
        juce::File chosenFile = chooser.getResult();
        
        djAudioPlayer->loadURL(juce::URL{chosenFile});
        waveformDisplay.loadURL(juce::URL{chosenFile});
    });
};


