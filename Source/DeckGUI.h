/*
  ==============================================================================

    DeckGUI.h
    Created: 22 Jan 2022 4:13:07pm
    Author:  oktay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "TrackListComponent.h"

//==============================================================================
/*
*/
class DeckGUI  :    public juce::Component,
                    public juce::Button::Listener,
                    public juce::Slider::Listener,
                    public juce::FileDragAndDropTarget,
                    public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& _formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, PlaylistComponent* _playlist);
    ~DeckGUI();

    void paint (juce::Graphics&) override;
    void resized() override;

    // Implement Button::Listener
    void buttonClicked(juce::Button*) override;

    // Implement Slider::Listener
    void sliderValueChanged(juce::Slider* slider) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override;

private:

    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD AND PLAY" };
    juce::TextButton playSelectedButton{ "PLAY PREPARED TRACK" };
    juce::TextButton muteButton{ "MUTE" };
    juce::TextButton twiceSpeedButton{ "2X" };
    juce::TextButton loadPlaylistButton{ "LOAD PLAYLIST" };

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    DJAudioPlayer* player;
    PlaylistComponent* playlist;
    WaveformDisplay waveformDisplay;
    

    TrackListComponent trackListComponent{ player, &waveformDisplay };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
