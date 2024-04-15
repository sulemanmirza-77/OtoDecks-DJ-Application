/*
  ==============================================================================

	PlaylistComponent.h
	Created: 07th February 2023 - 01:04 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "Track.h"
#include "Customize.h"
#include <vector>
#include <string>
#include <string.h>
#include <cmath>
#include <algorithm>
#include <fstream>

//==============================================================================
/* component that displays the track playlist and handles functions related to parsing file data */

class PlaylistComponent : public juce::Component,
	   					  public juce::TableListBoxModel,
						  public juce::Button::Listener,
						  public juce::TextEditor::Listener,
						  public juce::FileDragAndDropTarget
{
public:
	PlaylistComponent(DeckGUI* _deckGUI1,
					  DeckGUI* _deckGUI2);
	~PlaylistComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	// implement TableListBoxModel
	int getNumRows() override;
	void paintRowBackground(juce::Graphics& gfx, int rowNum, int width, int height, bool rowSelected) override;
	void paintCell(juce::Graphics& gfx, int rowNum, int columnId, int width, int height, bool rowSelected) override;
	juce::Component* refreshComponentForCell(int rowNum, int columnId, bool isRowSelected, juce::Component* UpdateExistingComponent) override;

	// implement Button::Listener
	void buttonClicked(juce::Button* button) override;

	// implement FileDragAndDropTarget
	bool isInterestedInFileDrag(const juce::StringArray& files) override;
	void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
	std::vector<Track> tracks{};
	std::vector<Track> searchHits{};
	
	DeckGUI* deckGUI1;
	DeckGUI* deckGUI2;
	
	juce::FileChooser fChooser{ "Select a file..." };
	juce::TextButton loadPlaylistButton{ "Load Playlist" };
	juce::TextEditor searchBox;
	juce::TableListBox tableComponent;

	Customize customize { this };

	// playlist function operations
	void searchPlaylist(juce::String searchText);
	int highlightTrack(juce::String searchText);
	void loadPlaylist();
	void handlePlaylistButtons(int buttonID);
	std::string secondsToMinutes(double seconds);
	juce::String getLengthMinutes(juce::URL audioURL);
	bool checkDupeTracks(juce::String fileName);

	// save session data when exiting program
	void saveSession();
	void loadLastSession();
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
