/*
  ==============================================================================

    FootstepSystem.cpp
    Created: 8 Jan 2026 2:47:17pm
    Author:  dbazurto

  ==============================================================================
*/

#include "FootstepSystem.h"

FootstepVoice* FootstepSystem::getFreeVoice()
{
    for (auto& voice : voices)
    {
        if (!voice.isActive())
        {
            return &voice;
        }
    }
    return &voices[0];
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
    // Implementation

	currentSampleRate = sampleRate;
	currentBlockSize = samplesPerBlock;

    for (auto& voice : voices)
    {
        voice.prepare(sampleRate);
    }

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

	//get surface type as int for now
	const int surfaceIndex = parameters.surfaceType.load(std::memory_order_relaxed);

	//Convert surface index to int for now (future: use enum class)
	SurfaceType surface = static_cast <SurfaceType>(surfaceIndex);

    //Set Surface Amount
	const float surfaceAmount = surfaceToBrightness(surface);

	//Parameter Smoothing Per Block
    constexpr float smoothing = 0.05f;

    smoothedSpeed += smoothing * (currentSpeed - smoothedSpeed);
    smoothedWeight += smoothing * (currentWeight - smoothedWeight);
    smoothedCloth += smoothing * (clothAmount - smoothedCloth);


    //Step Timing integration

    if (smoothedSpeed < 0.5f)
    {
		stepPhase = 0.0f; // Reset step phase when not moving
        return;
	}

    FootstepType currentType = getFootstepType(smoothedSpeed);

    float stepRate = getStepRateForType(currentType);

    advanceStepPhase(stepRate);

    if (consumeStep())
    {
        if (auto* voice = getFreeVoice())
        {
        voice->trigger(smoothedWeight,
                             surfaceAmount, // Placeholder for surface type
		                     smoothedCloth);

        }
        
    }

	//Avoiding triggers when not moving
    if (smoothedSpeed < minMovementSpeed)
    {
		stepPhase = 0.0f;
        return;
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

    //Micro-variation placeholder
	footstepRandom = random.nextFloat();

    /*future hooks :
     - Select surface
     - Reset envelope
     - Excite resonator */
}


