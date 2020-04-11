#pragma once

#include "AwarnessControl.h"
#include "IntelligentAwarness.h"
#include "AccessInfo.h"
#include "HaveDugInfo.h"
#include "HaveBarkedInfo.h"

class IntelligentMoveAwarness : public IntelligentAwarness{
private:
    AwarnessControl* AWC;
    HaveDugInfo *HDI;
    HaveBarkedInfo *HBI;
public:
    static constexpr int DEPTH_SIZE = 5;
    IntelligentMoveAwarness(AwarnessControl* AWC, const GameInfo *GI, HaveDugInfo *HDI, HaveBarkedInfo *HBI);
    float GetPossibleDigRate(const Cell& targetCell, Command nextDirect);
    void CreateAwarness();
    void UpdateAwarness(Awarness* awn);
    bool CheckComplate(Awarness* awn);
    Command JudgeCommand(Awarness* awn);
};