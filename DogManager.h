#pragma once

#include <vector>
#include "GameInfo.h"
#include "HaveDugInfo.h"
#include "HaveBarkedInfo.h"
#include "AccessInfo.h"
#include "AwarnessControlForDog.h"
#include "IntelligentStokingDogAwarness.h"
#include "IntelligentPetDogAwarness.h"

class DogManager
{
private:
	GameInfo* GI;
	AwarnessControlForDog* ACFA;
	signed int nextAction;
	HaveDugInfo* dugInfo;
	HaveBarkedInfo* barkedInfo;
	AccessInfo* ACI;
	IntelligentStokingDogAwarness* ISDA;
	IntelligentPetDogAwarness* IPDA;
    std::vector<GameInfo*>& GIHSTRY;
	
public:
	DogManager(AwarnessControlForDog* ACFA, GameInfo* GI, HaveDugInfo* HDI, HaveBarkedInfo* HBI, AccessInfo *ACI, std::vector<GameInfo*>& GIHSTRY);
	~DogManager();
	int GetNextAction(int SamuraiMove);
};

