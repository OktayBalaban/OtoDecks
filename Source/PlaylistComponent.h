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

//==============================================================================
/*
*/
class PlaylistComponent  :  public juce::Component,
                            public juce::TableListBoxModel,
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

private:

    juce::TableListBox tableComponent;
    std::vector<std::string> trackTitles;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
