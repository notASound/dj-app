#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
: formatManager(_formatManager)
{

};

DJAudioPlayer::~DJAudioPlayer(){};

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
};

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
};

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr)
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
    else
        std::cout << "Something went wrong loading the file" << std::endl;
};

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else
        transportSource.setGain(gain);
};

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.9 || ratio > 1.1)
    {
        std::cout << "DJAudioPlayer::setSpeed sain should be between 0.9 and 1.1" << std::endl;
    }
    else
        resampleSource.setResamplingRatio(ratio);
};

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
};

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1)
    {
        std::cout << "DJAudioPlayer::setPositionRelative: pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = pos * transportSource.getLengthInSeconds();
        setPosition(posInSecs);
    }
};

void DJAudioPlayer::start()
{
    transportSource.start();
};

void DJAudioPlayer::stop()
{
    transportSource.stop();
};

void DJAudioPlayer::cue()
{
    transportSource.setPosition(0);
};

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
};

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
};
