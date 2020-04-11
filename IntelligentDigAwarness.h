#pragma once

#include "AwarnessControl.h"
#include "IntelligentAwarness.h"
#include "AccessInfo.h"
#include "HaveDugInfo.h"

class IntelligentDigAwarness : public IntelligentAwarness{
private:
    AwarnessControl* AWC;
    AccessInfo* ASI;
    HaveDugInfo* HDI;
public:
    IntelligentDigAwarness(AwarnessControl* AWC, const GameInfo *GI, AccessInfo *ASI, HaveDugInfo* HDI);
    void CreateAwarness();
    void UpdateAwarness(Awarness* awn);
    bool CheckComplate(Awarness* awn);
    Command JudgeCommand(Awarness* awn);
};