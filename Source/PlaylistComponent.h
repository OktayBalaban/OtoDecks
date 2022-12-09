/*
  ==============================================================================

    PlaylistComponent.h
    Created: 23 Jan 2022 4:24:26pm
    Author:  oktay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "CSVOperator.h"
#include "TrackListComponent.h"

//==============================================================================
/*
*/
class PlaylistComponent  :  public juce::Component,
                                   juce::TableListBoxModel,
                                   juce::Button::Listener
{
    public:
        PlaylistComponent();
        ~PlaylistComponent();

        void paint (juce::Graphics&) override;
        void resized() override;

        int getNumRows() override;
        void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
        void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

        juce::Component* refreshComponentForCell(int rowNumber, int columnID, bool isRowSelected, juce::Component* existingComponentToUpdate) override;

        void buttonClicked(juce::Button* button) override;

        // Variables to store selected tracks and their paths
        juce::String selectedTrackPath;
        juce::String selectedTrack = "Nothing Selected";

        // Refresh the playlist
        void refreshPlaylist();

        // Converts a track path to track title
        juce::String convertTrackPathToTitle(std::string path);

        // Vectors to store tracks
        std::vector<juce::String> trackTitles;
        std::vector<std::string> trackPaths;

    private:

        juce::TableListBox tableComponent;

        juce::Label prepareLabel1; // Shows selected Track
        juce::Label prepareLabel2; // Writes Selected Track (Hardwritten)

        // Add - Remove uttons
        juce::TextButton addTrackButton{ "ADD NEW TRACK" };
        juce::TextButton removeTrackButton{ "REMOVE TRACK" };

        // Serachbox and Button
        juce::TextEditor searchBox{ "search tracks" };
        juce::TextButton searchButton{ "Find" };

        // Objects in order to get track Lengths
        juce::AudioFormatManager formatManager;

        // Populates track titles by converting track paths to titles
        void populateTrackTitles();

        // Search tracks
        void searchTracks(juce::String input);

        // Necessary Variables for a healthy search
        int rowCounter = -1;
        juce::String lastSearch = "";

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
