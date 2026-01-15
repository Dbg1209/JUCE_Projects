/*
  ==============================================================================

    FootstepSystem.cpp
    Created: 8 Jan 2026 2:47:17pm
    Author:  dbazurto

  ==============================================================================
*/

#include "FootstepSystem.h"

FootstepSystem::FootstepSystem()
{
    // Constructor implementation (if needed)
}

FootstepSystem::~FootstepSystem()
{
    // Destructor implementation (if needed)
}

void FootstepSystem::prepare(double sampleRate, int samplesPerBlock)
{
    // Implementation

	currentSampleRate = sampleRate;
	currentBlockSize = samplesPerBlock;

	isPrepared = true;
}

void FootstepSystem::process(juce::AudioBuffer<float>& buffer,
                             const FootstepParameters& parameters)
{
    // Implementation

    if (!isPrepared)
        return;

	//Retrieve atomic parameters
	float currentSpeed = parameters.speed.load();
	float currentWeight = parameters.weight.load();
	float clothAmount = parameters.clothIntensity.load();

	//Parameter Smoothing Per Block
    constexpr float smoothing = 0.05f;

    smoothedSpeed += smoothing * (currentSpeed - smoothedSpeed);
    smoothedWeight += smoothing * (currentWeight - smoothedWeight);
    smoothedCloth += smoothing * (clothAmount - smoothedCloth);


}