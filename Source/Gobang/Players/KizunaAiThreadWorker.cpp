#pragma once

#include "KizunaAiThreadWorker.h"
#include "GenericPlatform/GenericPlatformProcess.h"

uint32 FKizunaAiThreadWorker::RunProcess()
{
	FPlatformProcess::Sleep(FMath::RandRange(0.6f, 1.2f));
	if (board != nullptr) Result = ai->makeAction(*board);
	return 0;
}