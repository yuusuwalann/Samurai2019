#pragma once

#include "AwarnessControl.h"
#include "IntelligentAwarness.h"
#include "AccessInfo.h"

class IntelligentEscapeAwarness : public IntelligentAwarness{
private:
    AwarnessControl* AWC;
    AccessInfo* ASI;
public:
    IntelligentEscapeAwarness(AwarnessControl* AWC, const GameInfo *GI, AccessInfo *ASI);
    void CreateAwarness();
    void UpdateAwarness(Awarness* awn);
    bool CheckComplate(Awarness* awn);
    Command JudgeCommand(Awarness* awn);
};