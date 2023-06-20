#include "FileProcess.h"
#include <filesystem>

FileProcess::FileProcess(juce::File _file) : file(_file),
trackTitle(_file.getFileNameWithoutExtension()),
URL(juce::URL{_file})
{

}

bool FileProcess::operator==(const juce::String& other) const
{
    return trackTitle == other;
}

