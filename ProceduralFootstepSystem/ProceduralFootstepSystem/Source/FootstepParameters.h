/*
  ==============================================================================

    FootstepParameters.h
    Created: 15 Jan 2026 5:06:28pm
    Author:  dbazurto

  ==============================================================================
*/

#pragma once
#include <atomic>

struct FootstepParameters
{
	//Player Movement Parameters
	std::atomic<float> speed{ 0.8f };      // meters per second
	std::atomic<float> weight{ 1.0f };       // normalized 0 = light, 1 = heavy

	// Clothing Layers
	std::atomic<float> clothIntensity{ 0.0f }; // 0 = none, 1 = heavy

	// Surface type;
	std::atomic<int> surfaceType{ 0 };     // enum index for surface type (later to be replaced with enum class)
};

enum class SurfaceType : int
{
	Concrete = 0,
	Grass = 1,
	Metal = 2,
	Water = 3
};