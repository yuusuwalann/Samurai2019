#pragma once
#include "Agent.h"
#include "GameInfo.h"
#include <iostream>
#include <map>

class HaveBarkedInfo{
private:
    const GameInfo* GI;
    map<Cell, bool> haveBarkedMap;
    bool judgeOutFieldCell(const Cell& cell);
public:
    HaveBarkedInfo(GameInfo* GI);
    ~HaveBarkedInfo();
    void Update(GameInfo* GI);
	bool GetBarkedInfo(const Cell& cell);
    void DebugPrintMap();
};