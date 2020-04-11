#pragma once

#include "Awarness.h"
#include "IntelligentAwarness.h"
#include "GameInfo.h"
#include <iostream>
#include <vector>

class AwarnessControlForDog{
private:
    std::vector<Awarness*> awarnessList;
    Awarness* targetAwarness;
	GameInfo *GI;

public:
    AwarnessControlForDog(GameInfo* GI);
    void Update(GameInfo *gi);
    void PushAwarness(Awarness* aw);
    int DicideAwarness(int samuraiCmd);
    bool ExistAwarnessList(Awarness* aws);
};