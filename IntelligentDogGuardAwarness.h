#pragma once

#include "AwarnessControl.h"
#include "IntelligentAwarness.h"
#include "AccessInfo.h"

class IntelligentDogGuardAwarness : public IntelligentAwarness{
private:
    AwarnessControl* AWC;
    AccessInfo* ASI;
    const int INIT_INFO_VALUE = -1;
public:
    IntelligentDogGuardAwarness(AwarnessControl* AWC, const GameInfo *GI, AccessInfo *ASI);
    void CreateAwarness();
    void UpdateAwarness(Awarness* awn);
    bool CheckComplate(Awarness* awn);
    Command JudgeCommand(Awarness* awn);
};