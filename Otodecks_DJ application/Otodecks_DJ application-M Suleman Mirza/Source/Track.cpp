/*
  ==============================================================================

	Track.cpp
	Created: 18th February 2023 - 05:15 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#include "Track.h"
#include <filesystem>
using namespace juce;

/* class to handle the track object */
//initalize file type, title, and file path on object creation

Track::Track(File _file) :
	file{ _file },
	URL{ juce::URL{ _file } },
	title{ _file.getFileNameWithoutExtension().trim() },
	fileExtension{_file.getFileExtension()}

{
	DBG("Track::Track: Created new track: " << title << " from subpath: " << URL.getSubPath());
}

bool Track::operator==(const juce::String& track) const
{
	return title == track;
}