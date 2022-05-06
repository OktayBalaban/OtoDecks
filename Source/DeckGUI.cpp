/*
  ==============================================================================

    DeckGUI.cpp
    Created: 22 Jan 2022 4:13:07pm
    Author:  oktay

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "MainComponent.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& _formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, PlaylistComponent* _playlist)
                : player(_player), waveformDisplay(_formatManagerToUse, cacheToUse), playlist(_playlist)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playSelectedButton);
    addAndMakeVisible(loadPlaylistButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(twiceSpeedButton);
    addAndMakeVisible(trackListComponent);
    addAndMakeVisible(waveformDisplay);

    // Listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    playSelectedButton.addListener(this);
    loadPlaylistButton.addListener(this);
    muteButton.addListener(this);
    twiceSpeedButton.addListener(this);

    // Slider Ranges
    posSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.05, 2, 0.05);
    volSlider.setRange(0, 1, 0.01);

    startTimer(500);

    // Default Values for Sliders
    volSlider.setValue(1);
    speedSlider.setValue(1);

    // Make mute and 2x buttons toggleable and set their 'on' colours
    muteButton.setClickingTogglesState(true);
    muteButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::darkslategrey);
    twiceSpeedButton.setClickingTogglesState(true);
    twiceSpeedButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::darkslategrey);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

}

void DeckGUI::resized()
{

    // Variable Declaration for better grids
    double rowH = getHeight() / 20;
    double columnW = getWidth() / 16;

    // Button Bounds
    playButton.setBounds(columnW * 4, rowH, columnW * 4, rowH * 2);
    stopButton.setBounds(columnW * 8, rowH, columnW * 4, rowH * 2);
    playSelectedButton.setBounds(columnW * 4, rowH * 3, columnW * 8, rowH * 2);
    muteButton.setBounds(0, rowH * 4, columnW * 4, rowH);
    twiceSpeedButton.setBounds(columnW * 12, rowH * 4, columnW * 4, rowH);
    loadButton.setBounds(0, rowH * 19, columnW * 8, rowH * 1.4);
    loadPlaylistButton.setBounds(columnW * 8, rowH * 19, columnW * 8, rowH * 1.4);

    // Sliders
    volSlider.setBounds(0, 0, columnW * 4 , rowH * 4);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, columnW * 4, rowH * 0.75);

    speedSlider.setBounds(columnW * 12, 0, columnW * 4, rowH * 4);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, columnW * 4, rowH * 0.75);

    posSlider.setBounds(columnW * 0.1, rowH * 15, columnW * 15.9, rowH);
    posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, columnW * 4, rowH * 0.75);

    trackListComponent.setBounds(0, rowH * 5, getWidth(), rowH * 10);

    waveformDisplay.setBounds(columnW * 0.1, rowH * 16, columnW * 15.8, rowH * 3);


}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        player->start();

    }

    if (button == &stopButton)
    {
        player->stop();
    }

    // Loads and plays a track
    if (button == &loadButton)
    {
        juce::FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL{ chooser.getResult() });
            waveformDisplay.loadURL(juce::URL{ chooser.getResult() });

            // Update the Labels
            trackListComponent.currentTrack = chooser.getResult().getFileName();
            trackListComponent.nextTrack = "No Track";
            trackListComponent.previousTrack = "No Track";
            trackListComponent.labelUpdate();
            
            // Clean the playlist from the deck
            trackListComponent.trackTitles.clear();
            trackListComponent.trackPaths.clear();

        }

    }

    // Plays the prepared song from the playlist
    if (button == &playSelectedButton)
    {
        if (playlist->selectedTrack != "Nothing Selected")
        {
            // Plays the selected track
            player->loadURL(juce::URL{ juce::File::File(playlist->selectedTrackPath) });
            waveformDisplay.loadURL(juce::URL{ juce::File::File(playlist->selectedTrackPath) });

            // Update the Labels
            trackListComponent.currentTrack = playlist->selectedTrack;
            trackListComponent.nextTrack = "No Track";
            trackListComponent.previousTrack = "No Track";
            trackListComponent.labelUpdate();

            // Clean the playlist from the deck
            trackListComponent.trackTitles.clear();
            trackListComponent.trackPaths.clear();
        }

    }

    // Loads the playlist to the deck
    if (button == &loadPlaylistButton)
    {
        // In case there is no track, don't do anything
        if (playlist->trackTitles.size() > 0) 
        {
            trackListComponent.loadPlaylist(playlist->trackTitles, playlist->trackPaths);
        }

    }

    // Mutes the volume if on
    if (button == &muteButton)
    {
        if (muteButton.getToggleState())
        {
            player->setGain(0);
        }
        else
        {
            player->setGain(volSlider.getValue());
        }
    }

    // Set the speed 2x when 2x button is 'on'
    if (button == &twiceSpeedButton)
    {
        if (twiceSpeedButton.getToggleState())
        {
            player->setSpeed(2);
        }
        else
        {
            player->setSpeed(speedSlider.getValue());
        }
    }

}

// Slider Functions
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    // Changes the volume if mute is 'off'
    if (slider == &volSlider && !muteButton.getToggleState())
    {
        player->setGain(slider->getValue());
    }

    // Changes the speed if 2x is 'off'
    if (slider == &speedSlider && !twiceSpeedButton.getToggleState())
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    posSlider.setValue(player->getPositionRelative());
    trackListComponent.updateTimer(player->sendTimer());

}