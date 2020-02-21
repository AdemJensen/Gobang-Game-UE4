#pragma once

#include "KizunaAiThreadWorker.h"


uint32 FKizunaAiThreadWorker::Run()
{
	Result = Ai->makeAction();
	bFinished = true;
	return 0;
}