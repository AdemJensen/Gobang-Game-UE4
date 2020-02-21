#pragma once

#include "CoreMinimal.h"
#include "../Utilities/ThreadWorkerBase.h"
#include "../GobangFramework/Fubuki/Fubuki.h"

class GOBANG_API FFubukiThreadWorker : public FThreadWorkerBase
{
protected:
    bool bFinished = false;
    Fubuki* Ai;
    std::pair<int, int> Result = std::pair<int, int>(-1, -1);

public:
    void InitParam(Fubuki* AiObj) { Ai = AiObj; }

    virtual uint32 Run();

    std::pair<int, int> GetResult() { return Result; }
    bool IsOver() { return bFinished; }
};

