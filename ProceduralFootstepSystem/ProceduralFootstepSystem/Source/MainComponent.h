#pragma once

#include <JuceHeader.h>
#include "FootstepSystem.h"
#include "FootstepParameters.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

// Global Footstep Parameters Source


class MainComponent  : public juce::AudioAppComponent,
					   private juce::Slider::Listener,
					   private juce::ComboBox::Listener,
					   private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

	//Slider Listener
	void sliderValueChanged(juce::Slider* slider) override;

	//ComboBox Listener
	void comboBoxChanged(juce::ComboBox* comboBox) override;

	//Timer Callback
	void timerCallback() override;

private:
    //==============================================================================
    // Your private member variables go here...

	//Audio Configuration
	double currentSampleRate = 0.0;
	int currentBlockSize = 0;

	//Footstep System & Parameters
    FootstepParameters footstepParameters;
	FootstepSystem footstepSystem;

	//----------------------------------
	//GUI Components
	
    //Parameter Sliders
	juce::Slider speedSlider;
	juce::Label speedLabel;

	juce::Slider weightSlider;
	juce::Label weightLabel;

	juce::Slider clothSlider;
	juce::Label clothLabel;

	//Surface Type Selector
	juce::ComboBox surfaceTypeBox;
	juce::Label surfaceTypeLabel;

	//Visual Feedback on parameters
	juce::Label gaitTypeLabel; //Shows if it's sneaking, walking, running
	juce::Label stepPhaseLabel; //Debugs the step phase, from 0 to 1, on 1 should be resetting to 0
	juce::Label activeVoicesLabel; //Shows how many voices are active
	juce::Label currentStatusLabel; //Shows current status (Active/Idle)

	//Retrieve parameters for GUI update on atomic values
	std::atomic<int> currentGaitType{ 1 };      // 0=Sneak, 1=Walk, 2=Run
	std::atomic<float> currentStepPhase{ 0.0f };
	std::atomic<int> activeVoiceCount{ 0 };

	//Helper function to create and setup a slider
	void setupSlider(juce::Slider& slider, juce::Label& label, 
					 const juce::String& labelText, 
					 double min, double max, double defaultValue);

	//Helper function to create and setup Label for display
	void setupDisplayLabel(juce::Label& label, const juce::String& initialText);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
