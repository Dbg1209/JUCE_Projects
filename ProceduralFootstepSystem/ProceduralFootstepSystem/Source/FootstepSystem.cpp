/*
  ==============================================================================

    FootstepSystem.cpp
    Created: 8 Jan 2026 2:47:17pm
    Author:  dbazurto

  ==============================================================================
*/

#include "FootstepSystem.h"

namespace FootstepConstants
{
    constexpr float parameterSmoothing = 0.05f;
    constexpr float minMovementSpeed = 0.05f;
    constexpr float envSilenceThreshold = 0.0001f; 
}

FootstepVoice* FootstepSystem::getFreeVoice()
{
    for (auto& voice : voices)
    {
        if (!voice.isActive())
        {
            return &voice;
        }
    }
    return &voices[0]; //Right now is stealing the oldest voice (Future improvement to steal the quietest)
}

static float surfaceToBrightness(SurfaceType surface)
{
    switch (surface)
    {
    case SurfaceType::Concrete: return 1.0f;
    case SurfaceType::Grass:    return 0.3f;
    case SurfaceType::Metal:    return 1.2f;
    case SurfaceType::Water:    return 0.15f;
    default:                    return 1.0f;
    }
}

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

    //Parameter Validation
    if (sampleRate <= 0.0 || samplesPerBlock <= 0)
    {
        DBG("ERROR: Invalid audio parameters!");
        jassertfalse;
        return;
    }

    // Implementation

	currentSampleRate = sampleRate;
	currentBlockSize = samplesPerBlock;

    for (auto& voice : voices)
    {
        voice.prepare(sampleRate);
    }

	isPrepared = true;
    DBG("FootstepSystem prepared successfully - isPrepared = true");
}

void FootstepSystem::process(juce::AudioBuffer<float>& buffer,
                             const FootstepParameters& parameters)
{
    // Implementation

    if (!isPrepared)
        return;

	//Retrieve atomic parameters
	const float currentSpeed = parameters.speed.load(std::memory_order_relaxed);
	const float currentWeight = parameters.weight.load(std::memory_order_relaxed);
	const float clothAmount = parameters.clothIntensity.load(std::memory_order_relaxed);
	const int surfaceIndex = parameters.surfaceType.load(std::memory_order_relaxed);

	//Convert surface index to int for now (future: use enum class)
	SurfaceType surface = static_cast <SurfaceType>(surfaceIndex);
	const float surfaceAmount = surfaceToBrightness(surface);

	//Parameter Smoothing Per Block, this is now a Const value FootstepConstants::parameterSmoothing
    //constexpr float smoothing = 0.05f;

    smoothedSpeed += FootstepConstants::parameterSmoothing * (currentSpeed - smoothedSpeed);
    smoothedWeight += FootstepConstants::parameterSmoothing * (currentWeight - smoothedWeight);
    smoothedCloth += FootstepConstants::parameterSmoothing * (clothAmount - smoothedCloth);


    const bool isMoving = smoothedSpeed >= FootstepConstants::minMovementSpeed;


    //Movement check
    if (smoothedSpeed < FootstepConstants::minMovementSpeed)
    {
		stepPhase = 0.0f; // Reset step phase when not moving
        return;
	}
  
    DBG("Speed: " << juce::String(smoothedSpeed) + " Phase: " + juce::String(stepPhase));

    //Determine footstep type based on speed
    FootstepType currentType = getFootstepType(smoothedSpeed);

    //Determine Number of steps / second 
    float stepRate = getStepRateForType(currentType);

    //Advance step timing
    advanceStepPhase(stepRate);

    //Check if it's time for a new footstep
    if (consumeStep())
    {
        if (auto* voice = getFreeVoice())
        {
       
            const float clampedWeight = juce::jlimit(0.0f, 1.0f, smoothedWeight);
            const float clampedSurface = juce::jlimit(0.0f, 1.5f, surfaceAmount);
            const float clampedCloth = juce::jlimit(0.0f, 1.0f, smoothedCloth);

            voice->trigger(clampedWeight,
                           clampedSurface,
		                   clampedCloth);
            DBG("Voice triggered with clamped values");
        }

        else
        {
            DBG("No free voice available to trigger footstep");
		}
        
    }

	//Process Footstep Voice
    for (auto& voice : voices)
    {
        if (voice.isActive())
        {
        
        voice.process(buffer,
                      0,
		              buffer.getNumSamples());
        }
    }
}

FootstepType FootstepSystem::getFootstepType (float speed) const
{
    if (speed < 1.5f)
        return FootstepType::Sneak;
    else if (speed < 3.0f)
        return FootstepType::Walk;
    else
        return FootstepType::Run;
}

void FootstepSystem::advanceStepPhase(float stepRate)
{
    const float blockDuration = static_cast<float>(currentBlockSize) / static_cast<float>(currentSampleRate);

	stepPhase += stepRate * blockDuration;
}

bool FootstepSystem::consumeStep()
{
    if(stepPhase >= 1.0f)
    {
        stepPhase -= 1.0f;
        return true;
	}
    return false;
}

float FootstepSystem::getStepRateForType(FootstepType type) const
{
    switch (type)
    {
    case FootstepType::Sneak: return 1.2f;
    case FootstepType::Walk:  return 1.8f;
    case FootstepType::Run:   return 2.8f;
    default:                  return 1.8f;
    }
}

void FootstepSystem::triggerFootstep(FootstepType type)
{
	currentFootstepType = type;

	footstepEnergy = 1.0f;

     /*future hooks :
     - Select surface
     - Reset envelope
     - Excite resonator */
}


