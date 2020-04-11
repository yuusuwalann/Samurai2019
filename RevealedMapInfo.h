#pragma once
#include "Agent.h"
#include "GameInfo.h"
#include <iostream>
#include <map>

class RevealedMapInfo{
private:
    const GameInfo* GI;
    map<Cell, float> predicReveMap; //0-1
    map<Cell, float> heatRevMap; //0-1
    void updateHeatMap(const Cell& cell);
public:
    RevealedMapInfo(GameInfo* GI);
    ~RevealedMapInfo();
    void Update(GameInfo* GI);
	float GetPredicReveInfo(const Cell& cell);
    void DebugPrintMap();
};