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
#include "FootstepVoice.h"

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


	//Step timing 
	float stepPhase = 0.0f;
	void advanceStepPhase(float stepRate);
	bool consumeStep();

	// Determine footstep type based on speed
	FootstepType getFootstepType(float speed) const;
	float getStepRateForType(FootstepType type) const;

	//Get footstep voice Reference
	static constexpr int maxFootstepVoices = 8; //Set a max number of voices
	FootstepVoice voices[maxFootstepVoices]; //Pass the nax number of voices as an array pool, for re-use

	//Get free voices from the pool
	FootstepVoice* getFreeVoice();

	// Trigger footstep sound & variables
	void triggerFootstep(FootstepType type);

	float footstepEnergy = 0.0f;
	float footstepRandom = 0.0f;

	FootstepType currentFootstepType = FootstepType::Walk;

};