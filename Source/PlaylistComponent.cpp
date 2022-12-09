/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 23 Jan 2022 4:24:26pm
    Author:  oktay

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Reading tracks and populating the titles
    trackPaths = CSVOperator::returnTrackPathsArray();
    populateTrackTitles();

    tableComponent.getHeader().addColumn("Title", 1, 200);
    tableComponent.getHeader().addColumn("Length", 2, 200);
    tableComponent.getHeader().addColumn("", 3, 400);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(prepareLabel1);
    addAndMakeVisible(prepareLabel2);

    addAndMakeVisible(addTrackButton);
    addAndMakeVisible(removeTrackButton);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(searchButton);
    
    addTrackButton.addListener(this);
    removeTrackButton.addListener(this);

    formatManager.registerBasicFormats();
}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    

    // Prepare to play area
    g.setColour(juce::Colours::black);
    juce::Rectangle<float> prepareBorder(getWidth() * 0.295, getHeight() * 0.65, getWidth() * 0.41, getHeight() * 0.35);
    g.fillRect(prepareBorder);

    g.setColour(juce::Colours::darkslategrey);
    juce::Rectangle<float> prepareArea(getWidth() * 0.3, getHeight() * 0.68, getWidth() * 0.4, getHeight() * 0.31);
    g.fillRect(prepareArea);

    prepareLabel1.setColour(1, juce::Colours::white);
    prepareLabel1.setFont(20.0f);
    prepareLabel1.setText(selectedTrack, juce::dontSendNotification);
    prepareLabel1.setJustificationType(juce::Justification::centred);
    prepareLabel1.setBounds(getWidth() * 0.25, getHeight() * 0.80, getWidth() * 0.5, getHeight() * 0.1);

    prepareLabel2.setColour(1, juce::Colours::white);
    prepareLabel2.setFont(14.0f);
    prepareLabel2.setText("PREPARED TRACK", juce::dontSendNotification);
    prepareLabel2.setJustificationType(juce::Justification::centred);
    prepareLabel2.setBounds(getWidth() * 0.3, getHeight() * 0.72, getWidth() * 0.4, getHeight() * 0.1);

    // Add - Remove Track Buttons
    addTrackButton.setBounds(getWidth() * 0.72, getHeight() * 0.68, getWidth() * 0.25, getHeight() * 0.1);
    removeTrackButton.setBounds(getWidth() * 0.72, getHeight() * 0.80, getWidth() * 0.25, getHeight() * 0.1);

    // SearchBox and Search Button
    searchBox.setBounds(getWidth() * 0.02, getHeight() * 0.70, getWidth() * 0.25, getHeight() * 0.1);
    searchButton.setBounds(getWidth() * 0.15, getHeight() * 0.82, getWidth() * 0.12, getHeight() * 0.1);

    searchButton.addListener(this);
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    tableComponent.setBounds(0, 0, getWidth(), getHeight() * 0.67 );

    tableComponent.getHeader().setColumnWidth(1, getWidth() / 4);
    tableComponent.getHeader().setColumnWidth(2, getWidth() / 4);
    tableComponent.getHeader().setColumnWidth(3, getWidth() / 4);



}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::grey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (columnId == 1)
    {
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }

    if (columnId == 2)
    {
        juce::File filepath = trackPaths[rowNumber];
        int lengthInSeconds = 0;

        // Error handling in case the file format can not be read
        if (auto reader = formatManager.createReaderFor(filepath))
        {
            lengthInSeconds = reader->lengthInSamples / reader->sampleRate;

            // Prevent leaks
            delete reader;
        }
        else
        {
            g.drawText("Unknown", 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }
        
        // Converts raw seconds to a formatted time
        juce::String formattedTime = TrackListComponent::convertSecondsToTimer(lengthInSeconds);

        // Fill the cell
        g.drawText(formattedTime, 2, 0, width - 4, height, juce::Justification::centredLeft, true);

    }


}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnID, bool isRowSelected, juce::Component* existingComponentToUpdate)
{

    // Prepare to play buttons
    if (columnID == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "Prepare To Play" };
            juce::String id(std::to_string(rowNumber));
            btn->setComponentID(id);
            btn->setBounds(0, 0, 100, 22);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}


void PlaylistComponent::populateTrackTitles()
{
    juce::String title;

    // Converts the paths into track titles and add them to trackTitles
    for (std::string& s : trackPaths)
    {
        title = convertTrackPathToTitle(s);
        trackTitles.push_back(title);
    }
}

// Converts a track path to track title
juce::String PlaylistComponent::convertTrackPathToTitle(std::string path)
{

    // Gets the working folder name
    juce::File juceFilePath(path);

    // Substracts the title
    juce::String trackTitle = juceFilePath.getFileNameWithoutExtension();
     
    return trackTitle;
}

// Refresh the playlist after adding or removing a track
void PlaylistComponent::refreshPlaylist()
{
    //Resets and updates the data
    trackPaths = CSVOperator::returnTrackPathsArray();
    trackTitles.clear();
    populateTrackTitles();

    // Updates the content of the table
    tableComponent.updateContent();
}

// Search tracks
void PlaylistComponent::searchTracks(juce::String input)
{
    // First check for if the user searches for the last thing he searched or a new one
    // If it is new, the application will find the first result
    if (input != lastSearch)
    {
        rowCounter = -1;
    }

    // Updates last search
    lastSearch = input;

    // Searches for the input
    // rowCounter is set to -1 by default and the loop begins from rowCounter + 1
    for (int i = rowCounter + 1; i < trackTitles.size(); ++i)
    {
        if (trackTitles[i].containsIgnoreCase(input))
        {
            tableComponent.selectRow(i, false, true);
            rowCounter = i;            
            return;
        }

    }

    //Resets row counter if nothing has been found
    rowCounter = -1;

}


void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button->getButtonText() == "Prepare To Play")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG("Tracklist ID: " << trackTitles[id]);

        // Update selected track and selected track paths
        selectedTrackPath = trackPaths[id];
        selectedTrack = convertTrackPathToTitle(selectedTrackPath.toStdString());
        prepareLabel1.setText("SELECTED TRACK : " + selectedTrack, juce::dontSendNotification);

        DBG("TrackName: " << selectedTrack);
        DBG("TrackPath: " << selectedTrackPath);
    }

    if (button == &addTrackButton)
    {
        juce::FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            CSVOperator::addNewTrack(chooser.getResult().getFullPathName());
            refreshPlaylist();
        }
    }

    if (button == &removeTrackButton)
    {
        // Get the selected track's row and send it to deletion function in CSVOperator class
        int trackRowNumber = tableComponent.getSelectedRow();
        CSVOperator::removeTrack(trackRowNumber);

        // Refresh the playlist
        refreshPlaylist();
    }

    if (button == &searchButton)
    {
        searchTracks(searchBox.getText());
    }


}

