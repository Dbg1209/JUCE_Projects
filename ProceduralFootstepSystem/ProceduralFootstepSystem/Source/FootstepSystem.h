/*
  ==============================================================================

    FootstepSystem.h
    Created: 8 Jan 2026 2:47:17pm
    Author:  dbazurto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FootstepSystem
{
public:
	FootstepSystem();
	~FootstepSystem();


	// Called once when audio starts or device changes
	void prepare(double sampleRate, int samplesPerBlock);

	// Called every audio callback
	void process(juce::AudioBuffer<float>& buffer);
	

private:
	//Audio Configuration
	double currentSampleRate = 0.0;
	int currentBlockSize = 0;

	//Prepared flag
	bool isPrepared = false;
};