/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 15th January 2023 - 5:30 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <string>

/* class that contains the various functions of handling audio data */

class DJAudioPlayer : public juce::AudioSource
{
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        //==============================================================================
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;
        bool loadURL(juce::URL audioURL);

        // audio getter functions
        void start();
        void stop();
        bool isPlaying();
        double getLengthOfTrack();
        double getLengthAudioURL(juce::URL audioURL);
        double getCurrentPosition();
        double getPositionRelative();
        
        // audio setter functions
        void setPosition(double posInSeconds);
        void setPositionRelative(double pos);
        void setGain(double gain);
        void setSpeed(double ratio);
        bool toggleLooping();

        // IIRFilter filter passes
        void setFrequency(double frequency);
        void setLowShelf(double frequency);
        void setPeakFilter(double frequency);
        void setHighShelf(double frequency);

private:
    // load audio file dependency classes
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    juce::IIRFilterAudioSource filterSource {&resampleSource, false};
    juce::IIRFilterAudioSource lowSource{ &filterSource , false };
    juce::IIRFilterAudioSource midSource{ &lowSource , false };
    juce::IIRFilterAudioSource highSource{ &midSource , false };
    double globalSampleRate;
};