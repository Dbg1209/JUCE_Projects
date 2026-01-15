/*
  ==============================================================================

    FootstepSystem.h
    Created: 8 Jan 2026 2:47:17pm
    Author:  dbazurto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FootstepParameters.h"

class FootstepSystem
{
public:
	FootstepSystem();
	~FootstepSystem();


	// Called once when audio starts or device changes
	void prepare(double sampleRate, int samplesPerBlock);

	// Called every audio callback
	void process(juce::AudioBuffer<float>& buffer,
				 const FootstepParameters& parameters);
	

private:
	//Audio Configuration
	double currentSampleRate = 0.0;
	int currentBlockSize = 0;

	//Prepared flag
	bool isPrepared = false;

	//Smoothed runtime parameters

	float smoothedSpeed = 0.0f;
	float smoothedWeight = 0.0f;
	float smoothedCloth = 0.0f;

	
};