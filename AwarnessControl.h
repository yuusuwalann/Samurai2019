#pragma once

#include "Awarness.h"
#include "IntelligentAwarness.h"
#include "GameInfo.h"
#include <iostream>
#include <vector>

class AwarnessControl{
private:
    std::vector<Awarness*> awarnessList;
    Awarness* targetAwarness;
	GameInfo *GI;
public:
    AwarnessControl(GameInfo* GI);
    void Update(GameInfo *gi);
    void PushAwarness(Awarness* aw);
    int DicideAwarness();
    bool ExistAwarnessList(Awarness* aws);
};