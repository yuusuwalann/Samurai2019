#pragma once
#include "Agent.h"
#include "GameInfo.h"
#include <iostream>
#include <map>
#include <queue>
struct InfoStruct{
    int agentDist[4];
};

class AccessInfo : public Agent{
private:
    const int INIT_INFO_VALUE = -1;
    map<Cell, InfoStruct> accessInfoMap;
public:
    AccessInfo(GameInfo* GI);
    ~AccessInfo();
    void Update(GameInfo *GI);
	map<Cell, InfoStruct>* GetAccessMapAddr();
    int GetAccessDistance(const Cell& cell, int id);
    void DebugPrintMap(int id);
};