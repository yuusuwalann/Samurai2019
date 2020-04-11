#pragma once

#include "GameInfo.h"
#include "HaveDugInfo.h"
#include "HaveBarkedInfo.h"
#include "AccessInfo.h"
#include "RevealedMapInfo.h"
#include "MoveFreeInfo.h"
#include "AwarnessControl.h"
#include "IntelligentDigAwarness.h"
#include "IntelligentMoveAwarness.h"
#include "IntelligentEscapeAwarness.h"
#include "IntelligentChaisAwarness.h"
#include "IntelligentDogGuardAwarness.h"

/*
−1 現在位置に留まり, 何もしない.
0, …, 7 近傍セルのいずれかに動く.
8, …, 15 近傍セルのいずれかに穴を掘る.
16, …, 23 近傍セルのいずれかの穴を埋める.
*/

class SamuraiManager
{
private:
	const GameInfo* GI;
	signed int nextAction;
	HaveDugInfo* dugInfo;
	HaveBarkedInfo* barkedInfo;
	AccessInfo* accessInfo;
	RevealedMapInfo* revealedInfo;
	MoveFreeInfo* freeInfo;
	AwarnessControl* AWC;
	IntelligentDigAwarness* IDA;
	IntelligentMoveAwarness* IMA;
	IntelligentEscapeAwarness* IEA;
	IntelligentChaisAwarness* ICA;
	IntelligentDogGuardAwarness* IDGA;


public:
	SamuraiManager(const GameInfo* GI,AccessInfo* ACI, RevealedMapInfo* RMI,MoveFreeInfo* MFI,HaveDugInfo* HDI, HaveBarkedInfo* HBI, AwarnessControl* AWC);
	~SamuraiManager();
	int GetNextAction();
};

