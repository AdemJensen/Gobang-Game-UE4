#pragma once

#include "FubukiThreadWorker.h"


uint32 FFubukiThreadWorker::Run()
{
	Result = Ai->makeAction();
	bFinished = true;
	return 0;
}