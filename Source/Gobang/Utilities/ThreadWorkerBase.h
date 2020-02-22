#pragma once

#include "CoreMinimal.h"
#include "HAL/RunnableThread.h"
#include "HAL/Runnable.h"

class GOBANG_API FThreadWorkerBase : public FRunnable
{
protected:
    bool bFinished = false;
    FRunnableThread* Thread;

    // Begin FRunnable interface.
    virtual uint32 Run();
    virtual bool Init() { return true; }
    virtual void Stop();
    // End FRunnable interface

    virtual uint32 RunProcess() = 0;

public:
    void Start();
    void End();
    virtual bool IsOver() { return bFinished; }
};

