#pragma once

#include "CoreMinimal.h"
#include "HAL/RunnableThread.h"
#include "HAL/Runnable.h"
#include "../GobangFramework/Fubuki/Fubuki.h"

class GOBANG_API ThreadAiWorker : public FRunnable
{
private:
    bool isFinished = false;
    Fubuki* Ai;
    std::pair<int, int> result = std::pair<int, int>(-1, -1);
    FRunnableThread* Thread;

public:
    void initParam(Fubuki* AiObj) { Ai = AiObj; }

    void start() { Thread = FRunnableThread::Create(this, TEXT("Ai Worker"), 0, TPri_Normal); }

    // Begin FRunnable interface.
    virtual bool Init() { return true; }
    virtual uint32 Run();
    virtual void Stop() { isFinished = true; }
    // End FRunnable interface

    std::pair<int, int> getResult() { return result; }
    bool isOver() { return isFinished; }
};

