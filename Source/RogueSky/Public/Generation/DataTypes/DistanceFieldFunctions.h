#pragma once
#include "CoreMinimal.h"
#include "Generation/GenerationConstants.h"

namespace DistanceFieldFunctions {
	static float SmoothMax(float A, float B, float Smoothness) {
		A /= CHUNK_SIZE;
		B /= CHUNK_SIZE;

		float h = FMath::Clamp((B - A - Smoothness) / (2.0f * -Smoothness), 0.0f, 1.0f);
		return FMath::Clamp(A * h + B * (1.0f - h) + Smoothness * h * (1.0f - h), -1.0f, 1.0f) * CHUNK_SIZE;
	}

	static float SmoothMin(float A, float B, float Smoothness) {
		A /= CHUNK_SIZE;
		B /= CHUNK_SIZE;

		float h = FMath::Clamp((B - A + Smoothness) / (2.0f * Smoothness), 0.0f, 1.0f);
		return FMath::Clamp(A * h + B * (1.0f - h) - Smoothness * h * (1.0f - h), -1.0f, 1.0f) * CHUNK_SIZE;
	}
}