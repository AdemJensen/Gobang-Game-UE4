#pragma once

#include "KizunaAiThreadWorker.h"


uint32 FKizunaAiThreadWorker::RunProcess()
{
	if (board != nullptr) Result = ai->makeAction(*board);
	return 0;
}