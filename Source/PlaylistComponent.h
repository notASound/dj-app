#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "FileProcess.h"
#include <vector>
#include <string>

class PlaylistComponent: public juce::Component,
                         public juce::TableListBoxModel,
                         public juce::Button::Listener,
                         public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deck1,
                      DeckGUI* _deck2,
                      DJAudioPlayer* _metaData);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** get number of table rows */
    int getNumRows () override;
    
    /** draw the background of a row in the table*/
    void paintRowBackground (juce::Graphics &,
                             int rowNumber,
                             int width,
                             int height,
                             bool rowIsSelected) override;
    
    /** draws the contents of individual cells */
    void paintCell (juce::Graphics &,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;
    
    /** embedding components to a table cell*/
    Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        Component *existingComponentToUpdate) override;
    
    /** library button listener */
    void buttonClicked(juce::Button* button) override;

private:
    /** playlist - vector of FileProcess objects that stores information about tracks in playlist */
    std::vector<FileProcess> songs;
    
    DeckGUI* deck1;
    DeckGUI* deck2;
    DJAudioPlayer* djAudioPlayer;
    
    /** create buttons in playlist table */
    juce::TextButton addToPlaylistBtn{"Add to playlist... "};
    juce::TextButton clearPlaylistBtn{"Clear playlist"};
    juce::TextButton deck1Btn{"Load to deck 1"};
    juce::TextButton deck2Btn{"Load to deck 2"};
    
    /** create table with playlist */
    juce::TableListBox playlist;
    
    /** create search in playlist table */
    juce::TextEditor search;
    
    /** add songs to the playlist  */
    void importToPlaylist();
    
    /** clear playlist */
    void emptyPlaylist();
    
    /** open highlighted track on a selected deck*/
    void openOnDeck(DeckGUI* deckGUI);
    
    /** get track duration */
    juce::String getAudioLength(juce::URL audioURL);
    
    /** add search bar */
    void searchForSong(juce::String searchedSong);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
