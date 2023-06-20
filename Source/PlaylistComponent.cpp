#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DeckGUI.h"
#include <algorithm>
#include <fstream>

PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _metaData) :
                                    deck1(_deckGUI1),
                                    deck2(_deckGUI2),
                                    djAudioPlayer(_metaData)
{
    /** make elements visible */
    addAndMakeVisible(playlist);
    addAndMakeVisible(addToPlaylistBtn);
    addAndMakeVisible(clearPlaylistBtn);
    addAndMakeVisible(deck1Btn);
    addAndMakeVisible(deck2Btn);
    addAndMakeVisible(search);
    
    /** add listeners */
    addToPlaylistBtn.addListener(this);
    clearPlaylistBtn.addListener(this);
    deck1Btn.addListener(this);
    deck2Btn.addListener(this);
    search.addListener(this);
    
    /** add columns to playlist table*/
    playlist.getHeader().addColumn("Name", 1, 400);
    playlist.getHeader().addColumn("Duration", 2, 200);
    playlist.getHeader().addColumn("", 3, 200);
    playlist.setModel(this);
    
    /** search bar text */
    search.setTextToShowWhenEmpty("Search playlist...",
                                  juce::Colours::yellow);
    
    search.onTextChange = [this] { searchForSong(search.getText());
    };
}

PlaylistComponent::~PlaylistComponent()
{
    
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId)); // clear the background
    
    deck1Btn.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    deck1Btn.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
    deck2Btn.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    deck2Btn.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
    addToPlaylistBtn.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    addToPlaylistBtn.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
    
    clearPlaylistBtn.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    clearPlaylistBtn.setColour (juce::TextButton::textColourOffId, juce::Colours::yellow);
}

void PlaylistComponent::resized()
{
    float rowH = getHeight()/ 8;
    float columnW = getWidth() / 8;
    playlist.setBounds(0, 0, getWidth(), rowH * 7);
    deck1Btn.setBounds(0, rowH * 7, columnW * 1.5, rowH);
    deck2Btn.setBounds(columnW * 1.5, rowH * 7, columnW * 1.5, rowH);
    addToPlaylistBtn.setBounds(columnW * 3, rowH * 7, columnW * 1.5, rowH);
    clearPlaylistBtn.setBounds(columnW * 4.5, rowH * 7, columnW * 1.5, rowH);
    search.setBounds(columnW * 6, rowH * 7, columnW * 2, rowH);
}

int PlaylistComponent::getNumRows()
{
    return songs.size();
};

void PlaylistComponent::paintRowBackground (juce::Graphics & g,
                         int rowNumber,
                         int width,
                         int height,
                         bool rowIsSelected)
{
    if (rowIsSelected) // just highlight selected rows
    {
        g.fillAll(juce::Colours::yellow);
    }
    else
    {
        g.fillAll(juce::Colours::black);
    }
};

void PlaylistComponent::paintCell (juce::Graphics & g,
                int rowNumber,
                int columnId,
                int width,
                int height,
                bool rowIsSelected)
{
    
    if(columnId == 1) // track title
    {
        g.setColour(juce::Colours::yellow);
        if(rowIsSelected)
        {
            g.setColour(juce::Colours::black);
        }
        g.drawText (songs[rowNumber].trackTitle,
                    2, 0,
                    width - 4,
                    height,
                    juce::Justification::centredLeft,
                    true);
    }
    
    if(columnId == 2) // tracks duration
    {
        g.setColour(juce::Colours::yellow);
        if(rowIsSelected)
        {
            g.setColour(juce::Colours::black);
        }
        g.drawText (songs[rowNumber].trackLength,
                    2, 0,
                    width - 4,
                    height,
                    juce::Justification::centredLeft,
                    true);
    }
};

juce::Component* PlaylistComponent::refreshComponentForCell (int rowNumber,
                                    int columnId,
                                    bool isRowSelected,
                                    Component *existingComponentToUpdate)
{
    return existingComponentToUpdate;
};

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if(button == &addToPlaylistBtn)
    {
        importToPlaylist();
        playlist.updateContent();
    }
    
    if(button == &deck1Btn)
    {
        openOnDeck(deck1);
    }
    
    if(button == &deck2Btn)
    {
        openOnDeck(deck2);
    }
    
    if(button == &clearPlaylistBtn)
    {
        emptyPlaylist();
        playlist.updateContent();
    }
};

void PlaylistComponent::importToPlaylist()
{
    juce::FileChooser chooser{"Select files..."};
    if(chooser.browseForMultipleFilesToOpen())
    {
        for(const juce::File& file : chooser.getResults())
        {
            juce::String fileName{file.getFileNameWithoutExtension()};
            FileProcess fileToImport{file};
            juce::URL audioURL{file};
            fileToImport.trackLength = getAudioLength(audioURL);
            songs.push_back(fileToImport);
        }
    }
};

void PlaylistComponent::emptyPlaylist()
{
    songs.clear();
};

void PlaylistComponent::openOnDeck(DeckGUI* deckGUI)
{
    int highlightedRow{playlist.getSelectedRow()};
    if (highlightedRow != -1)
    {
        deckGUI->loadFile(songs[highlightedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                                          "Error #1",
                                          "Select a track in order to load on the deck",
                                          "OK",
                                          nullptr);
    }
};

juce::String PlaylistComponent::getAudioLength(juce::URL audioURL)
{
    djAudioPlayer->loadURL(audioURL);
    double seconds{ djAudioPlayer->getLengthInSeconds()};
    int convertedSeconds = int (seconds); // convert a double to an int
    juce::String minutesString{std::to_string(convertedSeconds / 60)};
    juce::String secondsString {std::to_string(convertedSeconds % 60)};
    return juce::String{minutesString + ":" + secondsString};
}

void PlaylistComponent::searchForSong(juce::String searchedSong)
{
    if(searchedSong != "")
    {
        auto it = find_if(songs.begin(), songs.end(),
                            [&searchedSong](const FileProcess& object) {return object.trackTitle.contains(searchedSong); });
        int rowNumber = -1;
        if(it != songs.end())
        {
            rowNumber = std::distance(songs.begin(), it);
            playlist.selectRow(rowNumber);
        }
        else
        {
            playlist.deselectAllRows();
        }
    }
};

