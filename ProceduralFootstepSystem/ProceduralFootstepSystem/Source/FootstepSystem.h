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

enum class FootstepType
{
	Sneak,
	Walk,
	Run
};

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

	// Determine footstep type based on speed

	FootstepType getFootstepType(float speed) const;

	static constexpr float sneakStepRate = 1.6f; // steps/second
	static constexpr float walkStepRate = 2.4f;  
	static constexpr float runStepRate = 3.2f;   

	//Step timing 

	float stepPhase = 0.0f;

	void advanceStepPhase(float stepRate);

	//Detect Step Event

	bool consumeStep();

	// Convert step type to rate
	float getStepRateForType(FootstepType type) const;

	// Trigger footstep sound & variables

	void triggerFootstep(FootstepType type);

	float footstepEnergy = 0.0f;
	float footstepRandom = 0.0f;

	FootstepType currentFootstepType = FootstepType::Walk;

	juce::Random random;

	//For avoiding triggers when not moving

	static constexpr float minMovementSpeed = 0.05f;


};