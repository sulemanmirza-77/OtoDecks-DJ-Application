/*
  ==============================================================================

	DJAudioPlayer.cpp
	Created: 15th January 2023 - 6:00 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#include "DJAudioPlayer.h"
using namespace juce;

/* class that contains the various functions of handling audio data */

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) :
	formatManager{ _formatManager },	
	globalSampleRate{ 0 }
{

}

DJAudioPlayer::~DJAudioPlayer() 
{

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	globalSampleRate = sampleRate;
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	filterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	lowSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	midSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	highSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	highSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
	highSource.releaseResources();
}

/* load audio track from file path */
bool DJAudioPlayer::loadURL(URL audioURL)
{
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

	if (reader != nullptr)
	{
		std::unique_ptr<AudioFormatReaderSource> newSource
		(new AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());

		DBG("DJAudioPlayer::loadURL: file successfully loaded");
		return true;
	}
	else {
		DBG("DJAudioPlayer::loadURL: unable to load file");
	}
	return false;
}

/* starts transportSource audio playback */
void DJAudioPlayer::start()
{
	transportSource.start();
}

/* stops transportSource audio playback */
void DJAudioPlayer::stop()
{
	transportSource.stop();
}

/* returns true if audio source is playing, false otherwise */
bool DJAudioPlayer::isPlaying()
{
	return transportSource.isPlaying();
}

/* returns data in seconds about length of track */
double DJAudioPlayer::getLengthOfTrack()
{
	return transportSource.getLengthInSeconds();
}

/* returns track duration in minutes without initializing transportSource */
double DJAudioPlayer::getLengthAudioURL(URL audioURL)
{
	double lengthInSeconds{ 0 };
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr)
	{
		std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
		// length of track = lengthInSamples / sampleRate
		lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
		newSource.reset();
	}
	return lengthInSeconds;
}

/* returns current track position in seconds*/
double DJAudioPlayer::getCurrentPosition()
{
	return transportSource.getCurrentPosition();
}

/* returns ratio of current track playback time*/
double DJAudioPlayer::getPositionRelative()
{
	return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

//==============================================================================
/* sets position of audioplay back, used in posSlider */
void DJAudioPlayer::setPosition(double posInSeconds)
{
	transportSource.setPosition(posInSeconds);
}

/* sets relative position of track ie. a truncated slider */
void DJAudioPlayer::setPositionRelative(double pos)
{
	if (pos < 0 || pos > 1.0)
	{
		DBG("DJAudioPlayer::setPositionRelative: position is out of set range");
	}
	else
	{
		double posInSeconds = transportSource.getLengthInSeconds() * pos;
		setPosition(posInSeconds);
	}
}

/* set volume of the audio track */
void DJAudioPlayer::setGain(double gain)
{
	if (gain < 0 || gain > 2.0)
	{
		DBG("DJAudioPlayer::setGain: gain is out of set range");
	}
	else
	{
		transportSource.setGain(gain);
	}
}

/* set playback speed of the track */
void DJAudioPlayer::setSpeed(double ratio)
{
	if (ratio < 0 || ratio > 100.0)
	{
		DBG("DJAudioPlayer::setSpeed: speed is out of set range");
	}
	else
	{
		resampleSource.setResamplingRatio(ratio);
	}
}

/* returns true if toggle was successful, looping is off by default */
bool DJAudioPlayer::toggleLooping()
{
	if (readerSource) // check audiosource exists
	{
		if (!readerSource->isLooping())
		{
			readerSource->setLooping(true);
			DBG("DJAudioPlayer::toggleLooping: looping toggled ON");
			return true;
		}
		else
		{
			readerSource->setLooping(false);
			DBG("DJAudioPlayer::toggleLooping: looping toggled OFF");
		}
	}
	return false;
}

//==============================================================================
/* sets coefficients of lowpass and highpass frequency for freqSlider */
void DJAudioPlayer::setFrequency(double frequency = 0)
{
	if (frequency < 0) 
	{
		IIRCoefficients lowPassFilter = IIRCoefficients::makeLowPass(globalSampleRate, frequency * -1);
		filterSource.setCoefficients(lowPassFilter);
		DBG("DJAudioPlayer::setLowPass: frequency: " << frequency * -1);
	}
	else if (frequency > 0)
	{
		IIRCoefficients highPassFilter = IIRCoefficients::makeHighPass(globalSampleRate, frequency);
		filterSource.setCoefficients(highPassFilter);
		DBG("DJAudioPlayer::setHighPass: frequency: " << frequency);
	}
	else
	{
		filterSource.makeInactive();
	}
}

/* sets coefficients of low shelf, changes output source */
void DJAudioPlayer::setLowShelf(double gainFactor = 1.0)
{
	IIRCoefficients lowShelf = IIRCoefficients::makeLowShelf(globalSampleRate, 300, 1.0 / juce::MathConstants<double>::sqrt2, gainFactor);
	lowSource.setCoefficients(lowShelf);
	DBG("DJAudioPlayer::setLowShelf: gainFactor: " << gainFactor);
}

/* sets coefficients of peak filter, changes output source */
void DJAudioPlayer::setPeakFilter(double gainFactor = 1.0)
{
	IIRCoefficients peakFilter = IIRCoefficients::makePeakFilter(globalSampleRate, 3000, 1.0 / juce::MathConstants<double>::sqrt2, gainFactor);
	midSource.setCoefficients(peakFilter);
	DBG("DJAudioPlayer::setPeakFilter: gainFactor: " << gainFactor);
}

/* sets coefficients of high shelf, changes output source */
void DJAudioPlayer::setHighShelf(double gainFactor = 1.0)
{
	IIRCoefficients highShelf = IIRCoefficients::makeHighShelf(globalSampleRate, 4500, 1.0 / juce::MathConstants<double>::sqrt2, gainFactor);
	highSource.setCoefficients(highShelf);
	DBG("DJAudioPlayer::setHighShelf: gainFactor: " << gainFactor);
}