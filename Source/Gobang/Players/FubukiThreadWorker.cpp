#pragma once

#include "FubukiThreadWorker.h"


uint32 FFubukiThreadWorker::RunProcess()
{
	Result = Ai->makeAction();
	return 0;
}