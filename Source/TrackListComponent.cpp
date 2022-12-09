/*
  ==============================================================================

    TrackListComponent.cpp
    Created: 19 Mar 2022 3:35:52pm
    Author:  oktay

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackListComponent.h"
#include "random"

//==============================================================================
TrackListComponent::TrackListComponent(DJAudioPlayer* _player, WaveformDisplay* _waveformDisplay)
                : player(_player), waveformDisplay(_waveformDisplay)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(repeatButton);
    addAndMakeVisible(shuffleButton);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(previousButton);

    addAndMakeVisible(previousLabel);
    addAndMakeVisible(previousLabel2);
    addAndMakeVisible(nextLabel);
    addAndMakeVisible(nextLabel2);
    addAndMakeVisible(currentTrackNameLabel);
    addAndMakeVisible(timerLabel);

    // Make repeat button toggleable
    repeatButton.setClickingTogglesState(true);
    repeatButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::darkslategrey);

    // Listeners
    nextButton.addListener(this);
    previousButton.addListener(this);
    shuffleButton.addListener(this);

    startTimer(1);
}

TrackListComponent::~TrackListComponent()
{
    player->releaseResources();
    stopTimer();
}

void TrackListComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::black);
    juce::Rectangle<float> prepareBorder(getWidth() * 0.03, getHeight() * 0.05, getWidth() * 0.92, getHeight() * 0.83);
    g.fillRect(prepareBorder);

    g.setColour(juce::Colours::darkslategrey);
    juce::Rectangle<float> prepareArea(getWidth() * 0.035, getHeight() * 0.06, getWidth() * 0.91, getHeight() * 0.81);
    g.fillRect(prepareArea);



}

void TrackListComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 10;
    double columnW = getWidth() / 16;

    nextButton.setBounds(columnW * 10, rowH * 9, columnW * 6, rowH);
    previousButton.setBounds(columnW * 0, rowH * 9, columnW * 6, rowH);
    shuffleButton.setBounds(columnW * 8, rowH * 9, columnW * 2, rowH);
    repeatButton.setBounds(columnW * 6, rowH * 9, columnW * 2, rowH);

    previousLabel.setColour(1, juce::Colours::white);
    previousLabel.setFont(10.0f);
    previousLabel.setText("PREVIOUS TRACK", juce::dontSendNotification);
    previousLabel.setJustificationType(juce::Justification::centred);
    previousLabel.setBounds(getWidth() * 0.05, getHeight() * 0.6, getWidth() * 0.3, getHeight() * 0.1);

    previousLabel2.setColour(1, juce::Colours::white);
    previousLabel2.setFont(14.0f);
    previousLabel2.setText(previousTrack, juce::dontSendNotification);
    previousLabel2.setJustificationType(juce::Justification::centred);
    previousLabel2.setBounds(getWidth() * 0.05, getHeight() * 0.7, getWidth() * 0.3, getHeight() * 0.1);


    nextLabel.setColour(1, juce::Colours::white);
    nextLabel.setFont(10.0f);
    nextLabel.setText("NEXT TRACK", juce::dontSendNotification);
    nextLabel.setJustificationType(juce::Justification::centred);
    nextLabel.setBounds(getWidth() * 0.65, getHeight() * 0.6, getWidth() * 0.3, getHeight() * 0.1);

    nextLabel2.setColour(1, juce::Colours::white);
    nextLabel2.setFont(14.0f);
    nextLabel2.setText(nextTrack, juce::dontSendNotification);
    nextLabel2.setJustificationType(juce::Justification::centred);
    nextLabel2.setBounds(getWidth() * 0.65, getHeight() * 0.7, getWidth() * 0.3, getHeight() * 0.1);

    currentTrackNameLabel.setColour(1, juce::Colours::white);
    currentTrackNameLabel.setFont(28.0f);
    currentTrackNameLabel.setText(currentTrack, juce::dontSendNotification);
    currentTrackNameLabel.setJustificationType(juce::Justification::centred);
    currentTrackNameLabel.setBounds(getWidth() * 0.1, getHeight() * 0.1, getWidth() * 0.8, getHeight() * 0.2);

    timerLabel.setColour(1, juce::Colours::white);
    timerLabel.setFont(28.0f);
    timerLabel.setText("0:00", juce::dontSendNotification);
    timerLabel.setJustificationType(juce::Justification::centred);
    timerLabel.setBounds(getWidth() * 0.1, getHeight() * 0.3, getWidth() * 0.8, getHeight() * 0.2);
}

// Updates the song names after a track change
void TrackListComponent::labelUpdate()
{
    nextLabel2.setText(nextTrack, juce::dontSendNotification);
    currentTrackNameLabel.setText(currentTrack, juce::dontSendNotification);
    previousLabel2.setText(previousTrack, juce::dontSendNotification);
}

// Loads the playlist into the deck and make the appropriate changes
void TrackListComponent::loadPlaylist(std::vector<juce::String> trackList, std::vector<std::string> pathList)
{
    // Loads the trackTitles and trackPath vectors
    trackTitles = trackList;
    trackPaths = pathList;

    // Resets the trackCounter
    trackCounter = 0;

    // Updates the track label variables
    //If there is no track playing, current track immediatly changes
    if (currentTrack == "No Track")
    {
        previousTrack = trackTitles.at(trackTitles.size() - 1);
        nextTrack = trackTitles.at(trackCounter + 1);
        currentTrack = trackTitles.at(trackCounter);

        player->loadURL(juce::URL{ juce::File::File(trackPaths.at(0)) });
        waveformDisplay->loadURL(juce::URL{ juce::File::File(trackPaths.at(0)) });
        player->stop();
    }
    // If there is a track playing, current track doesn't change and after pressing next song or previous song, the tracklist will be loaded
    else
    {
        trackCounter = -1;
        previousTrack = trackTitles.at(trackTitles.size() - 1);
        nextTrack = trackTitles.at(0);
    }

    // Update Labels
    labelUpdate();

}

void TrackListComponent::changeToNextTrack()
{

    // Change to track variables according to the playlist size
    if (trackTitles.size() >= 3)
    {
        if (trackCounter == 0)
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(0);
            nextTrack = trackTitles.at(2);
            currentTrack = trackTitles.at(1);

            // Updates tracCounter
            trackCounter = 1;
        }

        else if (trackCounter == trackTitles.size() - 1)
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(trackTitles.size() - 1);
            nextTrack = trackTitles.at(1);
            currentTrack = trackTitles.at(0);

            // Updates trackCounter
            trackCounter = 0;
        }

        else if (trackCounter == trackTitles.size() - 2)
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(trackTitles.size() - 2);
            nextTrack = trackTitles.at(0);
            currentTrack = trackTitles.at(trackTitles.size() - 1);

            // Updates trackCounter
            trackCounter++;
        }

        // The case of loading playlist while playing a track
        else if (trackCounter == -1)
        {
            previousTrack = trackTitles.at(trackTitles.size() - 1);
            nextTrack = trackTitles.at(1);
            currentTrack = trackTitles.at(0);

            trackCounter = 0;
        }

        else
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(trackCounter);
            nextTrack = trackTitles.at(trackCounter + 2);
            currentTrack = trackTitles.at(trackCounter + 1);

            // Updates trackCounter
            trackCounter++;
        }
    }

    else if (trackTitles.size() == 2)
    {
        if (trackCounter == 0)
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(0);
            nextTrack = trackTitles.at(0);
            currentTrack = trackTitles.at(1);

            // Updates trackCounter
            trackCounter = 1;
        }

        else if (trackCounter == -1)
        {
            previousTrack = trackTitles.at(1);
            nextTrack = trackTitles.at(1);
            currentTrack = trackTitles.at(0);

            trackCounter = 0;
        }

        else
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(1);
            nextTrack = trackTitles.at(1);
            currentTrack = trackTitles.at(0);

            trackCounter = 0;
        }
    }
    
    else if (trackTitles.size() == 1)
    {

        previousTrack = "No Track";
        nextTrack = "No Track";
        currentTrack = trackTitles.at(0);
    }

    else if (trackTitles.size() == 0)
    {
        return;
    }

    // Update Labels
    labelUpdate();

    // Play the track
    player->loadURL(juce::URL{ juce::File::File(trackPaths.at(trackCounter)) });
    waveformDisplay->loadURL(juce::URL{ juce::File::File(trackPaths.at(trackCounter)) });
}

void TrackListComponent::changeToPreviousTrack()
{
    // Change to track variables according to the playlist size
    if (trackTitles.size() >= 3)
    {
        if (trackCounter == 0)
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(trackTitles.size() - 2);
            nextTrack = trackTitles.at(0);
            currentTrack = trackTitles.at(trackTitles.size() - 1);

            // Updates tracCounter
            trackCounter = trackTitles.size() - 1;
        }

        else if (trackCounter == 1)
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(trackTitles.size() - 1);
            nextTrack = trackTitles.at(1);
            currentTrack = trackTitles.at(0);

            // Updates trackCounter
            trackCounter = 0;
        }

        else if (trackCounter == trackTitles.size() - 1)
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(trackTitles.size() - 3);
            nextTrack = trackTitles.at(trackTitles.size() - 1);
            currentTrack = trackTitles.at(trackTitles.size() - 2);

            // Updates trackCounter
            trackCounter--;
        }

        else if (trackCounter == -1)
        {
            previousTrack = trackTitles.at(trackTitles.size() - 2);
            nextTrack = trackTitles.at(0);
            currentTrack = trackTitles.at(trackTitles.size() - 1);

            trackCounter = trackTitles.size() - 1;
        }

        else
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(trackCounter - 2);
            nextTrack = trackTitles.at(trackCounter);
            currentTrack = trackTitles.at(trackCounter - 1);

            // Updates trackCounter
            trackCounter--;
        }
    }

    else if (trackTitles.size() == 2)
    {
        if (trackCounter == 0)
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(0);
            nextTrack = trackTitles.at(0);
            currentTrack = trackTitles.at(1);

            // Updates trackCounter
            trackCounter = 1;
        }

        else if (trackCounter == -1)
        {
            previousTrack = trackTitles.at(0);
            nextTrack = trackTitles.at(0);
            currentTrack = trackTitles.at(1);

            trackCounter = 1;
        }

        else
        {
            // Updates the track label variables
            previousTrack = trackTitles.at(1);
            nextTrack = trackTitles.at(1);
            currentTrack = trackTitles.at(0);

            trackCounter = 0;
        }
    }

    else if (trackTitles.size() == 1)
    {
        previousTrack = "No Track";
        nextTrack = "No Track";
        currentTrack = trackTitles.at(0);
    }

    else if (trackTitles.size() == 0)
    {
        return;
    }

    // Update Labels
    labelUpdate();

    // Play the track
    player->loadURL(juce::URL{ juce::File::File(trackPaths.at(trackCounter)) });
    waveformDisplay->loadURL(juce::URL{ juce::File::File(trackPaths.at(trackCounter)) });
}

void TrackListComponent::updateTimer(double time)
{
    juce::String timeAsString = convertSecondsToTimer(time);
    timerLabel.setText(timeAsString, juce::NotificationType::dontSendNotification);
}

// Takes a double time and returns it as a formatted string for timer
juce::String TrackListComponent::convertSecondsToTimer(double time)
{
    int sampleTime = floor(time);

    int hrs = sampleTime / 3600;
    int min = (sampleTime % 3600) / 60;
    int sec = sampleTime % 60;

    juce::String formattedTime;

    if (hrs != 0)
    {
        juce::String hrsAsStr(hrs);
        if (hrsAsStr.length() == 1)
        {
            hrsAsStr = "0" + hrsAsStr;
        }

        juce::String minAsStr(min);
        if (minAsStr.length() == 1)
        {
            minAsStr = "0" + minAsStr;
        }

        juce::String secAsStr(sec);
        if (secAsStr.length() == 1)
        {
            secAsStr = "0" + secAsStr;
        }

        formattedTime = hrsAsStr + ':' + minAsStr + ':' + secAsStr ;

        return formattedTime;
    }

    else
    {

        juce::String minAsStr(min);
        if (minAsStr.length() == 1)
        {
            minAsStr = "0" + minAsStr;
        }

        juce::String secAsStr(sec);
        if (secAsStr.length() == 1)
        {
            secAsStr = "0" + secAsStr;
        }

        formattedTime = minAsStr + ':' + secAsStr;

        return formattedTime;
    }
}

void TrackListComponent::shuffleList()
{
    // Shuffles the trackPaths vector first
    std::random_device rd;
    std::mt19937 shuffler(rd());
    std::shuffle(trackPaths.begin(), trackPaths.end(), shuffler);

    // The suffled trackTitles should be synchronized with trackPaths

    // Clear trackList vector first
    trackTitles.clear();

    juce::File tempPath;

    for (std::string& t : trackPaths)
    {
        // Convert t to File
        tempPath = juce::File(t);

        // Substract the file name and push to trackTitles vector
        trackTitles.push_back(tempPath.getFileNameWithoutExtension());
    }

    // Load the trackList again
    loadPlaylist(trackTitles, trackPaths);
}

void TrackListComponent::proceedEndOfTrack()
{
    // If repeat mode is not active, proceed to the next track
    if (!repeatButton.getToggleState())
    {
        changeToNextTrack();
    }
    // If repeat mode is active, play the same song again
    else
    {
        player->setPositionRelative(0.00);
        player->start();

    }

}


// Button function callers
void TrackListComponent::buttonClicked(juce::Button* button)
{
    if (button == &nextButton)
    {
        changeToNextTrack();
    }

    if (button == &previousButton)
    {
        changeToPreviousTrack();
    }

    if (button == &shuffleButton)
    {
        if (trackTitles.size() > 0)
        {
            shuffleList();
        }

    }
}

void TrackListComponent::timerCallback()
{
    if (currentTrack != "No Track")
    {
        if (player->isTrackFinished())
        {
            proceedEndOfTrack();
        }
    }
}