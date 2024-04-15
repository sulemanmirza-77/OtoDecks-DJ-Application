/*
  ==============================================================================

	Customize.cpp
	Created: 25th February 2023 - 11:59 PM
	Author:  Muhammad Suleman Mirza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Customize.h"
using namespace juce;

//==============================================================================
/* class to customize individual component parameters, as well as global lookandfeel */
Customize::Customize(Component* _component) :
	component{ _component }
{
	// global slider colours
	getLookAndFeel().setColour(Slider::thumbColourId, Colours::cyan.darker());
	getLookAndFeel().setColour(Slider::trackColourId, Colours::palegoldenrod);
	getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::magenta.darker(0.9f));
	getLookAndFeel().setColour(Slider::rotarySliderOutlineColourId, Colours::lightblue.darker(0.6f));
	getLookAndFeel().setColour(Slider::backgroundColourId, Colours::greenyellow.darker(0.6f));
	getLookAndFeel().setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
	getLookAndFeel().setColour(TextButton::buttonColourId, Colours::orange.darker(1.0f));
	getLookAndFeel().setColour(TextButton::buttonOnColourId, Colours::darkred.darker(1.0f));
	getLookAndFeel().setColour(ScrollBar::thumbColourId, Colours::deeppink.darker(1.2f));

	// slider colour set 1
	sliderColourSet1.setColour(Slider::thumbColourId, Colours::darkorange);
	sliderColourSet1.setColour(Slider::trackColourId, Colours::snow);
	sliderColourSet1.setColour(Slider::backgroundColourId, Colours::darkred);

}

//==============================================================================
/* set button parameters, makeVisible last */
void Customize::playButton(Button* button)
{
	const juce::String TEXT{ "Play" };

	button->setButtonText(TEXT);
	button->setClickingTogglesState(true);
	component->addAndMakeVisible(button);
}
void Customize::loopButton(Button* button)
{
	const juce::String TEXT{ "Repeat: Off" };

	button->setButtonText(TEXT);
	button->setClickingTogglesState(true);
	component->addAndMakeVisible(button);
}
void Customize::loadButton(Button* button)
{
	const juce::String TEXT{ "Load" };
	
	button->setButtonText(TEXT);
	component->addAndMakeVisible(button);
}


//==============================================================================
/* set slider parameters, rotary sliders are different components than linear sliders */
void Customize::volSlider(Slider* slider)
{
	const double MIN{ 0.0 };
	const double MAX{ 2.0 };
	const double DEFAULT_VAL{ 0.5 };

	slider->setRange(MIN, MAX);
	slider->setValue(DEFAULT_VAL);
	slider->setDoubleClickReturnValue(true, DEFAULT_VAL);
	slider->setTextValueSuffix("dB");
	slider->setNumDecimalPlacesToDisplay(1);
	slider->setTextBoxStyle(Slider::TextBoxRight, false, 50, 15);
	component->addAndMakeVisible(slider);
}

void Customize::speedSlider(Slider* slider)
{
	const double MIN{ 0.1 };
	const double MAX{ 3.0 };
	const double DEFAULT_VAL{ 1.0 };

	slider->setRange(MIN, MAX);
	slider->setValue(DEFAULT_VAL);
	slider->setDoubleClickReturnValue(true, DEFAULT_VAL);
	slider->setTextValueSuffix("x");
	slider->setNumDecimalPlacesToDisplay(2);
	slider->setTextBoxStyle(Slider::TextBoxRight, false, 50, 15);
	component->addAndMakeVisible(slider);
}

void Customize::freqSlider(Slider* slider)
{
	const double MIN{ -4999.9 };
	const double MAX{ 5000.0 };
	const double DEFAULT_VAL{ 0.0 };

	slider->setRange(MIN, MAX);
	slider->setValue(DEFAULT_VAL);
	slider->setDoubleClickReturnValue(true, DEFAULT_VAL);
	slider->setTextValueSuffix("Hz");
	slider->setNumDecimalPlacesToDisplay(0);
	slider->setTextBoxStyle(Slider::TextBoxRight, false, 50, 15);
	component->addAndMakeVisible(slider);
}
void Customize::posSlider(Slider* slider)
{
	const double MIN{ 0.0 };
	const double MAX{ 1.0 };
	const double DEFAULT_VAL{0.0};

	slider->setRange(MIN, MAX);
	slider->setValue(DEFAULT_VAL);
	slider->setTextValueSuffix("s");
	slider->setNumDecimalPlacesToDisplay(2);
	slider->setTextBoxStyle(Slider::TextBoxRight, false, 50, 15);
	
	slider->setLookAndFeel(&sliderColourSet1);
	component->addAndMakeVisible(slider);
}

void Customize::lowSlider(Slider* slider)
{
	const double MIN{ 0.01 };
	const double MAX{ 2.0 };
	const double DEFAULT_VAL{ 1.0 };

	slider->setRange(MIN, MAX);
	slider->setValue(DEFAULT_VAL);
	slider->setDoubleClickReturnValue(true, DEFAULT_VAL);
	slider->setNumDecimalPlacesToDisplay(1);
	slider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	slider->setSliderStyle(Slider::SliderStyle::Rotary);
	component->addAndMakeVisible(slider);
}
void Customize::midSlider(Slider* slider)
{
	const double MIN{ 0.01 };
	const double MAX{ 2.0 };
	const double DEFAULT_VAL{ 1.0 };

	slider->setRange(MIN, MAX);
	slider->setValue(DEFAULT_VAL);
	slider->setDoubleClickReturnValue(true, DEFAULT_VAL);
	slider->setNumDecimalPlacesToDisplay(1);
	slider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	slider->setSliderStyle(Slider::SliderStyle::Rotary);
	component->addAndMakeVisible(slider);
}
void Customize::highSlider(Slider* slider)
{
	const double MIN{ 0.01 };
	const double MAX{ 2.0 };
	const double DEFAULT_VAL{ 1.0 };

	slider->setRange(MIN, MAX);
	slider->setValue(DEFAULT_VAL);
	slider->setDoubleClickReturnValue(true, DEFAULT_VAL);
	slider->setNumDecimalPlacesToDisplay(1);
	slider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	slider->setSliderStyle(Slider::SliderStyle::Rotary);
	component->addAndMakeVisible(slider);
}


//==============================================================================
/* set label parameters, attached in main component does not require makeVisible */
void Customize::deckTitle(Label* label)
{
	label->setJustificationType(Justification::centred);
	label->setFont(20.0f);
	label->toBack();
	component->addAndMakeVisible(label);
}
void Customize::volLabel(Label* label)
{
	const juce::String TEXT{ "Volume" };

	label->setText(TEXT, dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->toBack();
	// attached in main component
}
void Customize::speedLabel(Label* label)
{
	const juce::String TEXT{ "Speed" };

	label->setText(TEXT, dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->toBack();
	// attached in main component

}
void Customize::freqLabel(Label* label)
{
	const juce::String TEXT{ "Freq" };

	label->setText(TEXT, dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->toBack();
	// attached in main component
}
void Customize::posLabel(Label* label)
{
	const juce::String TEXT{ "Time" };

	label->setText(TEXT, dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->toBack();
	// attached in main component
}
void Customize::lowLabel(Label* label)
{
	const juce::String TEXT{ "Bass" };

	label->setText(TEXT, dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->toBack();
	// attached in main component
}
void Customize::midLabel(Label* label)
{
	const juce::String TEXT{ "Balance" };

	label->setText(TEXT, dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->toBack();
	// attached in main component
}
void Customize::highLabel(Label* label)
{
	const juce::String TEXT{ "Treble" };

	label->setText(TEXT, dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->toBack();
	// attached in main component
}

//==============================================================================
/* set label parameters, attached in main component does not require makeVisible */
void Customize::searchBox(juce::TextEditor* editor)
{
	editor->setTextToShowWhenEmpty("Search", Colours::dimgrey);
	editor->setJustification(Justification::centred);
	editor->setFont(16.0f);
	component->addAndMakeVisible(editor);
}