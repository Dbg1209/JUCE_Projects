/*
  ==============================================================================

    FootstepVoice.cpp
    Created: 21 Jan 2026 2:00:38pm
    Author:  dbazurto

  ==============================================================================
*/

#include "FootstepVoice.h"


void FootstepVoice::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
}

void FootstepVoice::trigger(float weight,
                            float surface,
                            float cloth)
{
	//Initialize envelope and decay based on weight
	envelope = 1.0f;
	envelopeDecay = juce::jmap(weight, 
                               0.0f,    
                               1.0f, 
                               0.05f, 
                               0.95f);

	active = true;

	weightAmount = weight;
	surfaceAmount = surface;
	clothAmount = cloth;

}

void FootstepVoice::process(juce::AudioBuffer<float>& buffer,
    int startSample,
    int numSamples)
{
    //Decay envelope per sample
    envelope *= envelopeDecay;

    if (envelope < 0.0001f)
    {
        active = false;
    }

	//Impact Noise Generation
    float impact = random.nextFloat() * 2.0f - 1.0f; // Random impact variation
    impact *= envelope;
    impact *= juce::jmap(weightAmount,
                         0.0f,
                         1.0f,
                         0.2f,
                         1.2f);

    //Suface Filtering

    float cutoff = juce::jmap(surfaceAmount,
                              0.0f,
                              1.0f,
                              2000.0f,
		                      8000.0f);

	//Cloth Noise Generation
	float clothNoise = (random.nextFloat() * 2.0f - 1.0f) * clothAmount * envelope * 0.3f;

}

bool FootstepVoice::isActive() const
{
    return active;
}