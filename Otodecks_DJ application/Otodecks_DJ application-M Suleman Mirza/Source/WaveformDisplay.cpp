/*
  ==============================================================================

	WaveformDisplay.cpp
	Created: 2nd February 2023 - 06:33 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"
using namespace juce;

//==============================================================================
/* class that handles drawing and callback of the wave graphic */

WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
								 AudioThumbnailCache& cacheToUse) :
	audioThumb{ 1000, formatManagerToUse, cacheToUse },
	fileLoaded{ false },
	position{ 0 }
{
	audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{

}

/* paints main colours of the waveform graphic and playhead */
void WaveformDisplay::paint(Graphics& gfx)
{
	gfx.fillAll(Colours::darkgrey);   // the background is removed

	gfx.setColour(Colours::grey);
	gfx.drawRect(getLocalBounds(), 1);   // draw a border around the component

	if (fileLoaded)
	{
		gfx.setColour(Colours::blue); // draw waveform
		audioThumb.drawChannel(gfx, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);

		// draw current position indicator hand
		gfx.setColour(Colours::darkorange);
		
		// set width of indicator hand relative to length of track (in seconds)
		gfx.drawRect(position * getWidth(), 0, (int) std::max(3.0, ((getWidth() / 2) / audioThumb.getTotalLength()) + 2), getHeight());

		// set opaque colour to fill area passed
		gfx.setColour(Colours::black);
		gfx.setOpacity(0.5);
		gfx.fillRect(0, 0, position * getWidth(), getHeight());
	}
	else
	{
		gfx.setFont(20.0f);
		gfx.drawText("File not loaded...", getLocalBounds(), Justification::centred, true);   // draw the placeholder text
	}

}

void WaveformDisplay::resized()
{
}

/* loads the audio file into audioThumb that handles drawing the waveform */
void WaveformDisplay::loadURL(juce::URL audioURL)
{
	audioThumb.clear();
	fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
	if (fileLoaded)
	{
		DBG("WaveformDisplay::loadURL: loaded!");
		position = 0;
		repaint();
	}
	else {
		DBG("WaveformDisplay::loadURL: not loaded :(");
	}
}

/* listen for changes to waveform display, calls repaint(); */
void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
	repaint();
}

/* set the relative position of the playhead */
void WaveformDisplay::setPositionRelative(double pos)
{
	if (pos != position && pos > 0)
	{
		position = pos;
		repaint();
	}
}