#pragma once
#include <JuceHeader.h>

class FileProcess
{
public:
    FileProcess(juce::File _file);
    juce::File file;
    juce::URL URL;
    juce::String trackTitle;
    juce::String trackLength;
    bool operator==(const juce::String& other) const;
};
