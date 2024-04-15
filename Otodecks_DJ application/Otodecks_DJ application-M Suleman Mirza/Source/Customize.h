/*
  ==============================================================================

	Customize.h
	Created: 25th February 2023 - 02:07 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/* class to for variables component parameters and graphics options used in DeckGUI */
class Customize : public juce::Component
{
public:
	Customize(juce::Component* component);

	void playButton(juce::Button* button);
	void loopButton(juce::Button* button);
	void loadButton(juce::Button* button);

	void volSlider(juce::Slider* slider);
	void speedSlider(juce::Slider* slider);
	void posSlider(juce::Slider* slider);
	void freqSlider(juce::Slider* slider);
	void lowSlider(juce::Slider* slider);
	void midSlider(juce::Slider* slider);
	void highSlider(juce::Slider* slider);

	void deckTitle(juce::Label* label);
	void volLabel(juce::Label* label);
	void speedLabel(juce::Label* label);
	void freqLabel(juce::Label* label);
	void posLabel(juce::Label* label);
	void lowLabel(juce::Label* label);
	void midLabel(juce::Label* label);
	void highLabel(juce::Label* label);

	void searchBox(juce::TextEditor* editor);
	Component* component;

private:
	LookAndFeel_V4 sliderColourSet1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Customize)
};
