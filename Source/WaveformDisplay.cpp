#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse)
                                 : audioThumbnail(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0)
{
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::darkgrey);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::yellow);

    if (fileLoaded) {
        audioThumbnail.drawChannel(g,
                                   getLocalBounds(),
                                   0,
                                   audioThumbnail.getTotalLength(),
                                   0,
                                   1.0f);
        g.setColour(juce::Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 80, getHeight());
    }
    else {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (juce::Colours::black);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (juce::Colours::yellow);
        g.setFont (24.0f);
        g.drawText ("Load a file...", getLocalBounds(),
                    juce::Justification::centredLeft, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    
}

void WaveformDisplay::loadURL(juce::URL audioURL) {
    std::cout << "wfd: change received! " << std::endl;
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source) {
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position) {
        position = pos;
        repaint();
    }
}
