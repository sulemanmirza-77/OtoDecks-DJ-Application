/*
  ==============================================================================

	PlaylistComponent.cpp
	Created: 07th February 2023 - 11:54 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
using namespace juce;

//==============================================================================
/* component that displays the track playlist and handles functions related to parsing file data */

PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
									 DeckGUI* _deckGUI2) :
	deckGUI1{ _deckGUI1 },
	deckGUI2{ _deckGUI2 }
{
	// toolbar GUI components
	loadPlaylistButton.addListener(this);
	addAndMakeVisible(loadPlaylistButton);
	
	customize.searchBox(&searchBox);
	searchBox.addListener(this);
	searchBox.onTextChange = [this] { searchPlaylist(searchBox.getText()); };

	// modify table column headers
	tableComponent.getHeader().addColumn("Deck 1", 1, 50); // load deck 1 button
	tableComponent.getHeader().addColumn("Deck 2", 2, 50); // load deck 2 button
	tableComponent.getHeader().addColumn("Track Title", 3, 300);
	tableComponent.getHeader().addColumn("Length", 4, 350);
	tableComponent.getHeader().addColumn("File Ext.", 5, 350);
	tableComponent.getHeader().addColumn("Delete", 6, 50); // delete button

	tableComponent.setModel(this);
	tableComponent.getViewport()->setScrollBarsShown(true, false, false, false);
	addAndMakeVisible(tableComponent);
	loadLastSession();
}

PlaylistComponent::~PlaylistComponent()
{
	saveSession();
}

void PlaylistComponent::paint(Graphics& gfx)
{
	gfx.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // remove the background colour

	gfx.setColour(Colours::firebrick);
	gfx.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	gfx.setColour(Colours::white);
	gfx.setFont(14.0f);
	gfx.drawText("", getLocalBounds(), Justification::centred, true);   // Create some text to draw as a placeholder.
}

void PlaylistComponent::resized()
{
	int toolbarHeight = 40;
	loadPlaylistButton.setBounds(0, 0, getWidth() / 2, toolbarHeight);
	searchBox.setBounds(getWidth() / 2, 0, getWidth() / 2, toolbarHeight);

	tableComponent.setBounds(0, toolbarHeight, getWidth(), getHeight() - toolbarHeight);
	DBG("Height of playlist: " << getHeight());

	// dynamically resize playlist cells
	int colBlock = getWidth() / 16;
	tableComponent.getHeader().setColumnWidth(1, colBlock); // load deck 1 button
	tableComponent.getHeader().setColumnWidth(2, colBlock); // load deck 2 button
	tableComponent.getHeader().setColumnWidth(3, colBlock * 6);
	tableComponent.getHeader().setColumnWidth(4, colBlock * 5);
	tableComponent.getHeader().setColumnWidth(5, colBlock * 2);
	tableComponent.getHeader().setColumnWidth(6, colBlock); // delete button
}

/* returns the size of the current playlist, or if searching the search list */
int PlaylistComponent::getNumRows()
{
	if (!searchBox.isEmpty())
		return (int) searchHits.size();
	else
		return (int) tracks.size();
}

/* paint background colour of cells in the table class */
void PlaylistComponent::paintRowBackground(Graphics& gfx, int rowNum, int width, int height, bool rowSelected)
{
	// alternating colour effect
	if (rowSelected) {
		gfx.fillAll(Colours::darkorange);
	}
	else {
		if (fmod(rowNum, 2) == 0)
		{
			gfx.fillAll(Colours::grey.darker());
		}
		else {
			gfx.fillAll(Colours::grey);
		}
	}
}

/* draws other items into each table cell ie. text, calls tableComponent.updateContent() to refresh appearance */
void PlaylistComponent::paintCell(Graphics& gfx, int rowNum, int columnId, int width, int height, bool rowSelected)
{
		// performs different actions depending on column
		if (columnId == 3)
		{
			// load deck 1 text
			if (!searchBox.isEmpty())
			{
				if (rowNum < getNumRows()) // prevent vector out of range
					gfx.drawText(searchHits[rowNum].title, 2, 0, width - 4, height, Justification::centredLeft, true);
			}
			else
			{
				if (rowNum < getNumRows())
					gfx.drawText(tracks[rowNum].title, 2, 0, width - 4, height, Justification::centredLeft, true);
			}
		}
		if (columnId == 4)
		{
			// load deck 2 text
			if (!searchBox.isEmpty())
			{
				if (rowNum < getNumRows())
					gfx.drawText(searchHits[rowNum].length, 2, 0, width - 4, height, Justification::centredLeft, true);
			}
			else
			{
				if (rowNum < getNumRows())
					gfx.drawText(tracks[rowNum].length, 2, 0, width - 4, height, Justification::centredLeft, true);
			}
		}
		if (columnId == 5)
		{
			// delete button text
			if (!searchBox.isEmpty())
			{
				if (rowNum < getNumRows())
					gfx.drawText(searchHits[rowNum].fileExtension, 2, 0, width - 4, height, Justification::centredLeft, true);
			}
			else
			{
				if (rowNum < getNumRows())
					gfx.drawText(tracks[rowNum].fileExtension, 2, 0, width - 4, height, Justification::centredLeft, true);
			}
		}
}

/* draws buttons and listener objects for each cell in table list, also appends a unique buttonID */
Component* PlaylistComponent::refreshComponentForCell(int rowNum, int columnId, bool isRowSelected, Component* UpdateExistingComponent)
{
	// row iterates starting 0, column iterates starting 1
	if (columnId == 1 || columnId == 2)
	{
		// generate load deck 1 & deck 2 buttonID 
		if (UpdateExistingComponent == nullptr)
		{
			TextButton* btn = new TextButton{ "Load" };
			String id{ std::to_string(rowNum) + std::to_string(columnId) };
			btn->setComponentID(id);

			btn->addListener(this);
			UpdateExistingComponent = btn;
		}
	}
	if (columnId == 6)
	{
		// generate delete track buttonID 
		if (UpdateExistingComponent == nullptr)
		{
			TextButton* btn = new TextButton{ "x" };
			String id{ std::to_string(rowNum) + std::to_string(columnId) };
			btn->setComponentID(id);

			btn->addListener(this);
			UpdateExistingComponent = btn;
		}
	}
	return UpdateExistingComponent;
}

/* listener class to identify button interaction */
void PlaylistComponent::buttonClicked(Button* button)
{
	if (button == &loadPlaylistButton)
	{
		// opens file browser to select multiple files
		DBG("Load Playlist Button was clicked");
		loadPlaylist();
	}
	else 
	{
		// buttons in playlist were clicked, indentify by ID
		int buttonID = std::stoi(button->getComponentID().toStdString());
		DBG("PlaylistComponent::buttonClicked: " << button->getComponentID());

		handlePlaylistButtons(buttonID);
	}
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
	// allows for dragging of files into player window
	DBG("PlaylistComponent::isInterestedInFileDrag");
	return true;
}

/* parses audio file data and adds them to the playlist */
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
	// list of selected items stored as str array "files"
	if (files.size() >= 1)
	{
		for (const File& file : files)
		{
			juce::String fileName{ file.getFileNameWithoutExtension() };
			if (!checkDupeTracks(fileName)) // check if duplicate file loaded
			{
				// create new track object, save file path, parse track length, append track to tracks list
				Track createTrack{ file };
				juce::URL audioURL{ file };
				createTrack.length = getLengthMinutes(audioURL);
				tracks.push_back(createTrack);
				DBG("PlaylistComponent::filesDropped: dropped files loaded: " << createTrack.title);
			}
			else
			{
				// track already loaded
				DBG("PlaylistComponent::filesDropped: Duplicate file already loaded: " << fileName);
			}
		}
		// update library display after loading files
		tableComponent.updateContent();
	}
}


//==============================================================================
/* searches through current playlist for matching substring, and highlights row */
void PlaylistComponent::searchPlaylist(juce::String searchText)
{
	// search through Playlist, append search results to searchHits list to display
	DBG("PlaylistComponent::searchPlaylist: Searching for: " << searchText);
	if (searchText != "")
	{
		searchHits.clear();
		for (Track& track : tracks)
		{
			if (track.title.toLowerCase().contains(searchText.toLowerCase().trim()))
			{
				searchHits.push_back(track);
			}
		}
		int row = highlightTrack(searchText);
		tableComponent.selectRow(row);
		
		DBG("searchHits.size() results: " << std::to_string(searchHits.size()));
	}
	else
	{
		tableComponent.deselectAllRows();
	}
	tableComponent.updateContent();
	repaint();
}

/* returns index to highlight first row that matches search substring */
int PlaylistComponent::highlightTrack(juce::String searchText)
{
	// locate the index with searchText in title track 
	auto it = find_if(searchHits.begin(), searchHits.end(), [&searchText](const Track& obj) 
		{return obj.title.toLowerCase().contains(searchText.toLowerCase().trim()); });
	int i = -1;

	if (it != searchHits.end())
	{
		i = (int) std::distance(searchHits.begin(), it);
	}

	return i;
}

/* opens file browser and parses selected file(s) into playlist */
void PlaylistComponent::loadPlaylist()
{
	// opens file browser to select multiple files
	FileChooser chooser{ "Select files..." };
	if (chooser.browseForMultipleFilesToOpen())
	{
		for (const File& file : chooser.getResults())
		{
			juce::String fileName{ file.getFileNameWithoutExtension() };
			if (!checkDupeTracks(fileName)) // check if duplicate file loaded
			{
				// create new track object, save file path, parse track length, append track to tracks list
				Track createTrack{ file };
				juce::URL audioURL{ file };
				createTrack.length = getLengthMinutes(audioURL);
				tracks.push_back(createTrack);
				DBG("PlaylistComponent::buttonClicked: file loaded: " << createTrack.title);
			}
			else 
			{
				// track already loaded
				DBG("PlaylistComponent::buttonClicked: Duplicate file already loaded: " << fileName);
			}
		}
		// update library display after loading files
		tableComponent.updateContent();
	}
}


/* identify component buttons by parsing buttonId (in a 2 number format : row, col) */
void PlaylistComponent::handlePlaylistButtons(int buttonID)
{
	int row = floor(buttonID / 10); // row is first digit
	int col = fmod(buttonID, 10);   // column is second digit

	// load deck 1
	if (col == 1)
	{
		if (!searchBox.isEmpty()) // if there is a search query, read searchHits instead
		{
			if (deckGUI1->player->loadURL(searchHits[row].URL)) {
				deckGUI1->togglePlayButton();
			}
			deckGUI1->waveformDisplay.loadURL(searchHits[row].URL);
			deckGUI1->deckTitle.setText(searchHits[row].title, dontSendNotification);
		}
		else
		{
			if (deckGUI1->player->loadURL(tracks[row].URL)) {
				deckGUI1->togglePlayButton();
			}
			deckGUI1->waveformDisplay.loadURL(tracks[row].URL);
			deckGUI1->deckTitle.setText(tracks[row].title, dontSendNotification);
		}
	}

	// load deck 2
	if (col == 2)
	{
		if (!searchBox.isEmpty())
		{
			if (deckGUI2->player->loadURL(searchHits[row].URL)) {
				deckGUI2->togglePlayButton();
			}
			deckGUI2->waveformDisplay.loadURL(searchHits[row].URL);
			deckGUI2->deckTitle.setText(searchHits[row].title, dontSendNotification);
		}
		else
		{
			if (deckGUI2->player->loadURL(tracks[row].URL)) {
				deckGUI2->togglePlayButton();
			}
			deckGUI2->waveformDisplay.loadURL(tracks[row].URL);
			deckGUI2->deckTitle.setText(tracks[row].title, dontSendNotification);
		}
	}

	// delete button
	if (col == 6)
	{
		if (!searchBox.isEmpty())
		{
			DBG("PlaylistComponent::buttonClicked: searching for track, delete disabled");
		}
		else
		{
			tracks.erase(tracks.begin() + row);
			DBG("PlaylistComponent::buttonClicked: track " << row << " deleted");
		}
		tableComponent.updateContent();
	}
}

/* parse time from a double into minute string format : "00:00" */
std::string PlaylistComponent::secondsToMinutes(double seconds)
{
	seconds = std::round(seconds);

	std::string minString{ "" };
	std::string secString{ "" };

	int min{ int(seconds / 60) };
	int sec{ int(std::fmod(seconds, 60)) };

	std::cout << "min: " << min << ", sec: " << sec << std::endl;

	// adds an extra 0 infront if digits are less than 10
	if (min < 10)
	{
		minString = "0" + std::to_string(min);
	}
	else
	{
		minString = std::to_string(min);
	}
	if (sec < 10)
	{
		secString = "0" + std::to_string(sec);
	}
	else
	{
		secString = std::to_string(sec);
	}

	std::cout << "minString: " << minString << ", secString: " << secString << std::endl;

	return std::string(minString + ":" + secString);
}

/* asks DJAudioplayer for audio length, then parses data calling by secondsToMinutes() */
String PlaylistComponent::getLengthMinutes(juce::URL audioURL)
{
	double lengthInSeconds = deckGUI1->player->getLengthAudioURL(audioURL);
	std::string lengthInMinutes = secondsToMinutes(lengthInSeconds);

	DBG("PlaylistComponent::getLengthMinutes: track length: " << lengthInMinutes);
	return juce::String(lengthInMinutes);
}

/* checks playlist if file name already exists in playlist */
bool PlaylistComponent::checkDupeTracks(juce::String fileName)
{
	return (std::find(tracks.begin(), tracks.end(), fileName) != tracks.end());
}


//==============================================================================
/* save playlist data when exiting program, uses fstream */
void PlaylistComponent::saveSession()
{
	// Make a .csv file in order to store the playlist.
	std::ofstream savedPlaylist("saved-playlist.csv");

	// save the playlist to a file
	for (Track& track : tracks)
	{
		savedPlaylist << track.file.getFullPathName() << "," << track.length << "\n";
	}
}

/* load playlist data when opening program, reads specific .csv file in same directory */
void PlaylistComponent::loadLastSession()
{
	// generate an input stream using a previously saved session
	std::ifstream savedPlaylist("saved-playlist.csv");
	std::string filePath;
	std::string length;

	// read the data, and then construct new objects line by line.
	if (savedPlaylist.is_open())
	{
		while (getline(savedPlaylist, filePath, ',')) {
			File file{ filePath };
			Track newTrack{ file };

			getline(savedPlaylist, length);
			newTrack.length = length;
			tracks.push_back(newTrack);
		}
	}
	savedPlaylist.close();
}