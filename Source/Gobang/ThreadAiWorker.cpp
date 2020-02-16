#pragma once

#include "ThreadAiWorker.h"


uint32 ThreadAiWorker::Run()
{
	result = Ai->makeAction();
	isFinished = true;
	return 0;
}