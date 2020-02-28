#pragma once

#include "CoreMinimal.h"
#include "../Utilities/ThreadWorkerBase.h"
#include "../GobangFramework/KizunaAi/KizunaAi.h"
#include "../GobangFramework/Board.h"

class GOBANG_API FKizunaAiThreadWorker : public FThreadWorkerBase
{
protected:
    KizunaAi* ai;
    Board* board;
    std::pair<int, int> Result = std::pair<int, int>(-1, -1);

public:
    void InitParam(KizunaAi* AiObj, Board* BoardObj) { ai = AiObj; board = BoardObj; }

    virtual uint32 RunProcess();

    std::pair<int, int> GetResult() { return Result; }
};

