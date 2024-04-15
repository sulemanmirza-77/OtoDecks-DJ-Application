/* If encountering any difficulties enable in Projucer options:
    - using namespace juce; enabled
    - JUCE_MODAL_LOOPS_PERMITTED=1 under preprocessor definitions
 */

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
class NewProjectApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    NewProjectApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // The app's initialization code would go in this function.

        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        // The code that will terminate the application will come here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // When the app asks to quit, you can ignore it or call quit() to close it.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {

    }

    //==============================================================================
    /*
        This class is responsible for the implementation of the desktop window
        that holds an instance of our MainComponent class.
    */
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro is responsible for generating the main() procedure, which is then used to launch the application.
START_JUCE_APPLICATION (NewProjectApplication)