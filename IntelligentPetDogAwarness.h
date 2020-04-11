#pragma once

#include "AwarnessControlForDog.h"
#include "IntelligentAwarness.h"
#include "AccessInfo.h"
#include "HaveDugInfo.h"

class IntelligentPetDogAwarness : public IntelligentAwarness{
private:
    AwarnessControlForDog* AWC;
    AccessInfo* ASI;
    HaveDugInfo* HDI;
public:
    IntelligentPetDogAwarness(AwarnessControlForDog* AWC, const GameInfo *GI, AccessInfo *ASI, HaveDugInfo* HDI);
    void CreateAwarness();
    void UpdateAwarness(Awarness* awn);
    bool CheckComplate(Awarness* awn);
    Command JudgeCommand(Awarness* awn);
};