/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MyFirstAudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                private juce::Slider::Listener //Callback Function Inheritance [2]
{
public:
    MyFirstAudioPluginAudioProcessorEditor (MyFirstAudioPluginAudioProcessor&);
    ~MyFirstAudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void sliderValueChanged(juce::Slider* slider) override; // This is the default callback function [3]
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyFirstAudioPluginAudioProcessor& audioProcessor;
    //Slider 
    juce::Slider midiVolume; // [1]


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyFirstAudioPluginAudioProcessorEditor)
};
