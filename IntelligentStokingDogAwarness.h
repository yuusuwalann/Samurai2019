#pragma once

#include "AwarnessControlForDog.h"
#include "IntelligentAwarness.h"
#include "AccessInfo.h"

class IntelligentStokingDogAwarness : public IntelligentAwarness{
private:
    AwarnessControlForDog* AWC;
    AccessInfo* ASI;
    std::vector<GameInfo*>& GIHSTRY;
public:
    IntelligentStokingDogAwarness(AwarnessControlForDog* AWC, const GameInfo *GI, AccessInfo *ASI, std::vector<GameInfo*>& GIHSTRY);
    void CreateAwarness();
    void UpdateAwarness(Awarness* awn);
    bool CheckComplate(Awarness* awn);
    Command JudgeCommand(Awarness* awn);
};