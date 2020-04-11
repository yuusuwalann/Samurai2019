#pragma once
#include "Agent.h"
#include "GameInfo.h"
#include <iostream>
#include <map>

class HaveDugInfo{
private:
    GameInfo* GI;
    map<Cell, bool> haveDugMap;
public:
    HaveDugInfo(GameInfo* GI);
    ~HaveDugInfo();
    void Update(GameInfo* GI);
	bool GetDugInfo(const Cell& cell);
    void DebugPrintMap();
};