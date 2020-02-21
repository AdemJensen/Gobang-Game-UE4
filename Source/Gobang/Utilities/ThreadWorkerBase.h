#pragma once

#include "CoreMinimal.h"
#include "HAL/RunnableThread.h"
#include "HAL/Runnable.h"

class GOBANG_API FThreadWorkerBase : public FRunnable
{
protected:
    bool isFinished = false;
    FRunnableThread* Thread;

public:
    void start() { Thread = FRunnableThread::Create(this, TEXT("Thread Worker"), 0, TPri_Normal); }

    // Begin FRunnable interface.
    virtual bool Init() { return true; }
    virtual uint32 Run() = 0;
    virtual void Stop() { isFinished = true; }
    // End FRunnable interface
    virtual bool isOver() { return isFinished; }
};

