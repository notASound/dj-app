#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource
{

public:
  DJAudioPlayer(juce::AudioFormatManager& _formatManager);  
  ~DJAudioPlayer();

    /**  implement pure virtual functions from AudioAppComponent class that requires implementation in inheritance class MainComponent */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    /** loads the track */
    void loadURL(juce::URL audioURL);

    /** change the tracks volume */
    void setGain(double gain);

    /** change the tracks speed */
    void setSpeed(double ratio);

    /** change position where the track is played from */
    void setPosition(double posInSecs);

    /** set relative position in the track inside sliders range  */
    void setPositionRelative(double pos);

    /** start playing the track*/
    void start();

    /** stop playing the track*/
    void stop();
    
    /** cue the track */
    void cue();
    
    /** get the relative position of the play head */
    double getPositionRelative();
    
    /** get length of a track added to playlist in seconds */
    double getLengthInSeconds();

private:
  juce::AudioFormatManager& formatManager;
  std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
  juce::AudioTransportSource transportSource;
  juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
