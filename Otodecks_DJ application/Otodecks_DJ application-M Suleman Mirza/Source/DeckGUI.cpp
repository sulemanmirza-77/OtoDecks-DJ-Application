/*
  ==============================================================================

	DeckGUI.cpp
	Created: 21th January 2023 - 10:16 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
using namespace juce;

//==============================================================================
/* class that handles main GUI components of the DJplayer, mainly buttons and sliders */

DeckGUI::DeckGUI(DJAudioPlayer* _player,
	AudioFormatManager& formatManagerToUse,
	AudioThumbnailCache& cacheToUse) :
	player{ _player },
	waveformDisplay{ formatManagerToUse, cacheToUse }
{
	// timer tick rate
	startTimer(150);

	// play button
	playButton.addListener(this);
	customize.playButton(&playButton);

	// repeat button
	loopButton.addListener(this);
	customize.loopButton(&loopButton);

	// load button
	loadButton.addListener(this);
	customize.loadButton(&loadButton);

	// vol slider & label
	volSlider.addListener(this);
	volLabel.attachToComponent(&volSlider, true);
	customize.volSlider(&volSlider);
	customize.volLabel(&volLabel);

	// speed slider & label
	speedSlider.addListener(this);
	speedLabel.attachToComponent(&speedSlider, true);
	customize.speedSlider(&speedSlider);
	customize.speedLabel(&speedLabel);

	// frequency slider & label
	freqSlider.addListener(this);
	freqLabel.attachToComponent(&freqSlider, true);
	customize.freqSlider(&freqSlider);
	customize.freqLabel(&freqLabel);

	// playback position slider & label
	posSlider.addListener(this);
	posLabel.attachToComponent(&posSlider, true);
	customize.posSlider(&posSlider);
	customize.posLabel(&posLabel);

	// IIRF low slider & label
	lowSlider.addListener(this);
	lowLabel.attachToComponent(&lowSlider, false);
	customize.lowSlider(&lowSlider);
	customize.lowLabel(&lowLabel);

	// IIRF mid slider & label
	midSlider.addListener(this);
	midLabel.attachToComponent(&midSlider, false);
	customize.midSlider(&midSlider);
	customize.midLabel(&midLabel);


	// IIRF high slider & label
	highSlider.addListener(this);
	highLabel.attachToComponent(&highSlider, false);
	customize.highSlider(&highSlider);
	customize.highLabel(&highLabel);

	// title display label
	customize.deckTitle(&deckTitle);

	// waveform component
	addAndMakeVisible(waveformDisplay);
}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::paint(Graphics& gfx)
{
	gfx.fillAll(Colours::black.darker(0.7));   // clear the background}
}

void DeckGUI::resized()
{
	double rowH = getHeight() / 11;
	// buttons, GUI components in format: x,  y,  width,  height
	loadButton.setBounds(0, 0, getWidth() / 3, rowH);
	playButton.setBounds(getWidth() / 3, 0, getWidth() / 3, rowH);
	loopButton.setBounds(getWidth() / 3 * 2, 0, getWidth() / 3, rowH);

	// sliders
	volSlider.setBounds(50, rowH * 2, getWidth() - 65, rowH);
	speedSlider.setBounds(50, rowH * 3 - 10, getWidth() - 65, rowH);
	highSlider.setBounds(getWidth() / 3 * 2, rowH * 4 - 5, getWidth() / 3, rowH * 3 - 5);
	midSlider.setBounds(getWidth() / 3, rowH * 4 - 5, getWidth() / 3, rowH * 3 - 5);
	lowSlider.setBounds(0, rowH * 4 - 5, getWidth() / 3, rowH * 3 - 5);
	freqSlider.setBounds(50, rowH * 7 - 10, getWidth() - 65, rowH);
	posSlider.setBounds(50, getHeight() - rowH, getWidth() - 65, rowH);

	// labels
	deckTitle.setBounds(0, rowH + 8, getWidth(), rowH);
	highLabel.setBounds(getWidth() / 3 * 2, rowH * 4, getWidth() / 3, rowH * 3 - 28);
	midLabel.setBounds(getWidth() / 3, rowH * 4, getWidth() / 3, rowH * 3 - 28);
	lowLabel.setBounds(0, rowH * 4, getWidth() / 3, rowH * 3 - 28);

	// waveform
	waveformDisplay.setBounds(0, getHeight() - rowH * 3, getWidth(), rowH * 2);
}

/* checks which button is clicked by validating pointer address */
void DeckGUI::buttonClicked(Button* button)
{
	if (button == &playButton)
	{
		togglePlayButton();
	}
	if (button == &loopButton)
	{
		toggleLoopButton();
	}
	if (button == &loadButton)
	{
		// opens file browser and parses selected files
		auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
		fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
			{
				auto chosenFile = chooser.getResult();

				// call both audio player and waveform display functions
				if (player->loadURL(URL{ chosenFile })) {
					playButton.setButtonText("Play");
				}
				waveformDisplay.loadURL(URL{ chosenFile });
				deckTitle.setText(chosenFile.getFileNameWithoutExtension(), dontSendNotification);
			});
	}
}

/* checks if audio source is currently playing, toggles playButton */
void DeckGUI::togglePlayButton()
{
	if (!player->isPlaying())
	{
		if (posSlider.getValue() >= posSlider.getMaximum() - 2)
		{
			// rewind slider if finishing playing
			posSlider.setValue(0.0);
		}
		player->start();
		DBG("Play Start");
	}
	else
	{
		player->stop();
		DBG("Play Stop");
	}
}

/* checks if audio source is set to loop, toggles loopButton */
void DeckGUI::toggleLoopButton()
{
	player->toggleLooping() ? loopButton.setButtonText("Loop: On") : loopButton.setButtonText("Loop: Off");
}

/* listener handler for slider components, identified by reference */
void DeckGUI::sliderValueChanged(Slider* slider)
{
	if (slider == &volSlider)
	{
		// set volume
		player->setGain(slider->getValue());
	}
	if (slider == &speedSlider)
	{
		// set playback speed
		player->setSpeed(slider->getValue());
	}
	if (slider == &posSlider)
	{
		// set time in seconds of the track
		player->setPosition(slider->getValue());
		if (player->isPlaying())
		{
			// set slider value to length of track
			posSlider.setRange(0.0, player->getLengthOfTrack());
			posSlider.setNumDecimalPlacesToDisplay(2);
		}
	}
	if (slider == &freqSlider)
	{
		// set frequency
		player->setFrequency(slider->getValue());
	}
	if (slider == &highSlider)
	{
		// set low pass frequency
		player->setHighShelf(slider->getValue());
	}
	if (slider == &midSlider)
	{
		// set low pass frequency
		player->setPeakFilter(slider->getValue());
	}
	if (slider == &lowSlider)
	{
		// set low pass frequency
		player->setLowShelf(slider->getValue());
	}
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
	// allows for dragging of files into player window
	DBG("DeckGUI::isInterestedInFileDrag");
	return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
	// list of selected items stored as str array "files"
	for (String filename : files)
	{
		DBG("DeckGUI::filesDropped " << filename);
	}
	if (files.size() >= 1)
	{
		// add to table list instead
		// perhaps add to main component or allow drag in table
		player->loadURL(juce::URL{ File{files[0]} });
		waveformDisplay.loadURL(URL{ File{files[0]} });
		deckTitle.setText(File{ files[0] }.getFileNameWithoutExtension(), dontSendNotification);
	}
}

/* timer function to continually update audio callback */
void DeckGUI::timerCallback()
{
	if (!isnan(player->getPositionRelative()) && player->isPlaying())
	{
		// move posSlider together according to track position 
		waveformDisplay.setPositionRelative(player->getPositionRelative());
		posSlider.setValue(player->getCurrentPosition());
		playButton.setButtonText("Stop");
	}
	else
		playButton.setButtonText("Play");
}
