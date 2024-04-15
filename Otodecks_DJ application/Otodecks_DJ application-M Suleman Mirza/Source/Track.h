/*
  ==============================================================================

	Track.h
	Created: 18th February 2023 - 12:25 AM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/* class to handle the track object */

class Track
{
public:
	Track(juce::File _file);
	juce::File file;
	juce::URL URL;
	juce::String title;
	juce::String length;
	juce::String fileExtension;

	// enable comparison search operations
	bool operator==(const juce::String& other) const;
};