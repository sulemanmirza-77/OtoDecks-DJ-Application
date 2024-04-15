#include "MainComponent.h"
using namespace juce;

//==============================================================================
/* main class container head for other components */

MainComponent::MainComponent()
{
    // After adding any child components, adjust the size of the component.
    setSize (1250, 700);

    // Request authorization to open input channels on some systems.
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Set the number of input/output channels.
        setAudioChannels (2, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    deckGUI1.deckTitle.setText("Deck 1 Screen", dontSendNotification);
    deckGUI2.deckTitle.setText("Deck 2 Screen", dontSendNotification);
    
    addAndMakeVisible(playlistComponent);

    // register audio file formats
    formatManager.registerBasicFormats();

}

MainComponent::~MainComponent()
{
    // This turns off the audio device and clears the source.
    shutdownAudio();
}

//==============================================================================
/* prepares the source to play */
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // call players in mixed audio channel
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

/* called this repeatedly to retrieve additional audio data */
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

/* lets the source discard anything it doesn't require after playback. */
void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
/* components override this method to draw their content */
void MainComponent::paint (Graphics& gfx)
{
    gfx.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

/* invoked when this component's size has changed, such as window resizing */
void MainComponent::resized()
{
    //set deckGUI 1 & 2 to half size of main component
    //                              x, y,          width,      height
    deckGUI1.setBounds(             0, 0, getWidth() / 4, getHeight() );
    deckGUI2.setBounds(getWidth() * 0.25, 0, getWidth() / 4, getHeight() );

    playlistComponent.setBounds(getWidth() * 0.50 , 0, getWidth() * 0.50, getHeight());

    DBG("MainComponent::resized");
}

