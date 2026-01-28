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

    filterCoeff = 0.5f;
    filterState = 0.0f;
}

void FootstepVoice::trigger(float weight,
                            float surface,
                            float cloth)
{
    //Checks:
    DBG("=== VOICE TRIGGERED ===");
    DBG("Weight: " + juce::String(weight) + " Surface: " + juce::String(surface) + " Cloth: " + juce::String(cloth));

	weightAmount = juce::jlimit(0.f, 1.f, weight);
	surfaceAmount = juce::jlimit(0.f, 1.5f, surface);
	clothAmount = juce::jlimit(0.f, 1.f, cloth);

    
	//Initialize envelope and decay based on weight
    envelope = 1.0f;


	envelopeDecay = juce::jmap(weight, 
                               0.0f,    
                               1.0f, 
                               0.95f, 
                               0.9995f);
    DBG("Envelope: " + juce::String(envelope) + " Decay: " + juce::String(envelopeDecay));

	active = true;

    //Calculate filter coefficient based on surface brightness

    float cutoffHz = juce::jmap(surfaceAmount,
                           0.0f,
                           1.0f,
                           800.0f,   
                           6000.0f);
    DBG("Filter cutoff: " + juce::String(cutoffHz) + " Hz");

    //Convert cutoff freq to filter coef
    float omega = juce::MathConstants<float>::twoPi * cutoffHz / static_cast<float>(currentSampleRate);
    filterCoeff = 1.0f - std::exp(-omega);

    //Limit coef to a safe range
    filterCoeff = juce::jlimit(0.01f, 0.99f, filterCoeff);

    //Reset filter for next step
    filterState = 0.f;

    DBG("Voice is now ACTIVE");
}

void FootstepVoice::process(juce::AudioBuffer<float>& buffer,
    int startSample,
    int numSamples)
{

    const int numChannels = buffer.getNumChannels();
    const int endSample = startSample + numSamples;


    //Process each sample
    for (int sample = startSample; sample < endSample; ++sample)
    {
        //Generate raw impact noise
        float rawNoise = randomFloat() * 2.f - 1.f;

        //Scale by envelope
        float impact = rawNoise * envelope;

        //Scale by weight (heavier = louder)
        impact *= juce::jmap(weightAmount,
                             0.0f,
                             1.0f,
                             0.2f, //light footsteps
                             1.2f);//Heavier footsteps

        //Applying surface filters

        filterState += filterCoeff * (impact - filterState);
        float filteredImpact = filterState;

        //Generate Cloth Noise

        float clothNoise = (randomFloat() * 2.0f - 1.0f) * clothAmount * envelope * 0.3f;

        //Combine Layers and send to Buffer

        float finalSample = filteredImpact + clothNoise;

        //Soft clipping to prevent digital clipping

        finalSample = std::tanh(finalSample);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            buffer.addSample(channel, sample, finalSample);
        }

        //Decay envelope (Fadeout)
        envelope *= envelopeDecay;

        if (envelope < 0.0001f)
        {
            active = false;
            break;
        }
    }
}

bool FootstepVoice::isActive() const
{
    return active;
}

float FootstepVoice::randomFloat()
{
    // std::minstd_rand is a simple linear congruential generator
    // It's fast and doesn't use any locks or shared state
    randomGen = (randomGen * 48271) % 2147483647;

    // Convert to 0.0 to 1.0 range
    return static_cast<float>(randomGen) / 2147483647.0f;
}