#include "StdAfx.h"
#include "MathUtils.h"
#include "GamePlugin.h"

int32 MathUtils::GetRandomInt(int32 min, int32 max)
{
	int32 range = max - min + 1;
	return rand() % range + min;
}

f32 MathUtils::GetRandomFloat(f32 min, f32 max)
{
	return (min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / max - min)));
}