/*
  ==============================================================================

    TrackListComponent.h
    Created: 19 Mar 2022 3:35:52pm
    Author:  oktay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <iostream>
#include <string>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
//==============================================================================
/*
*/
class TrackListComponent : public   juce::Component,
                                    juce::Button::Listener,
                                    juce::Timer
          
{
public:
    TrackListComponent(DJAudioPlayer* _player, WaveformDisplay* _waveformDisplay);
    ~TrackListComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // Implement Button::Listener
    void buttonClicked(juce::Button*) override;

    void timerCallback() override;


    // Track Variables

    std::vector<juce::String> trackTitles;
    std::vector<std::string> trackPaths;

    juce::String previousTrack = "No Track";
    juce::String nextTrack = "No Track";
    juce::String currentTrack = "No Track";

    // Variable to keep track of song list
    int trackCounter = 0;

    // Updates the song names after a track change
    void labelUpdate();

    // Loads the playlist into the deck and make the appropriate changes
    void loadPlaylist(std::vector<juce::String> trackList, std::vector<std::string> pathList);

    // Update Track Time
    void updateTimer(double time);

    // Takes a double time and returns it as a formatted string for timer
    static juce::String convertSecondsToTimer(double time);

    // Actions after an end of track
    void proceedEndOfTrack();


private:

    // Buttons
    juce::TextButton nextButton{ "NEXT" };
    juce::TextButton previousButton{ "PREVIOUS" };
    juce::TextButton shuffleButton{ "SHUFFLE" };
    juce::TextButton repeatButton{ "REPEAT" };

    // Labels
    juce::Label previousLabel;
    juce::Label previousLabel2;
    juce::Label nextLabel;
    juce::Label nextLabel2;
    juce::Label currentTrackNameLabel = currentTrack;
    juce::Label timerLabel;

    DJAudioPlayer* player;
    WaveformDisplay* waveformDisplay;

    // Functions to change songs
    void changeToNextTrack();
    void changeToPreviousTrack();

    // Shuffles the tracklist
    void shuffleList();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackListComponent)
};
