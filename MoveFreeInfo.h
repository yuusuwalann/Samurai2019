#pragma once
#include "Agent.h"
#include "GameInfo.h"
#include <iostream>

class MoveFreeInfo: public Agent{
private:
    float moveRate[4]; // 0:束縛 - 1:Free
    void updateMoveRate();
public:
    MoveFreeInfo(GameInfo* GI);
    ~MoveFreeInfo();
    void Update(GameInfo* GI);
	float GetMoveRate(int id);
    void DebugPrintMap();
};