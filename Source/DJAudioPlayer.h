/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 22 Jan 2022 11:02:25am
    Author:  oktay

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class DJAudioPlayer : public juce::AudioSource {
    public:

        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        //==============================================================================
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(juce::URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
       
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);

        void start();
        void stop();

        // Get the relative position of the playhead
        double getPositionRelative();

        double sendTimer();

        // Sends if the track is finished or not
        bool isTrackFinished();


    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};

};