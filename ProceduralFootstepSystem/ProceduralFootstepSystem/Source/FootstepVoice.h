/*
  ==============================================================================

    FootstepVoice.h
    Created: 21 Jan 2026 2:00:38pm
    Author:  dbazurto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FootstepVoice
{
public:
    void prepare(double sampleRate);
	void trigger(float weight,
                 float surface,
                 float cloth);
    void process(juce::AudioBuffer<float>& buffer,
                 int startSample,
		         int numSamples);
    bool isActive() const;

private:
	//Reference for sample rate
    double currentSampleRate = 0.0;

    //Envelope parameters

    float envelope = 0.0f;
    float envelopeDecay = 0.0f;
	bool active = false;

	//Random generator for variation
    uint32_t randomGen = 1;

    //Helper function for generating random floats
    float randomFloat();

	//Cache parameter values for current footstep

    float weightAmount = 1.0f;
    float surfaceAmount = 0.0f;
    float clothAmount = 0.0f;

    //Filter state for surface

    float filterState = 0.0f;
    float filterCoeff = 0.5f;
};  

