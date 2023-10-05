#include "StdAfx.h"
#include "MathUtils.h"
#include "GamePlugin.h"

int32 MathUtils::GetRandomInt(int32 min, int32 max)
{
	int32 range = max - min + 1;
	return rand() % range + min;
}