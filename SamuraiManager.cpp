#include "SamuraiManager.h"

using namespace std;

SamuraiManager::SamuraiManager(const GameInfo *GI, AccessInfo *ACI, RevealedMapInfo *RMI, MoveFreeInfo *MFI, HaveDugInfo *HDI, HaveBarkedInfo *HBI, AwarnessControl* AWC)
	: GI(GI), accessInfo(ACI), revealedInfo(RMI), freeInfo(MFI), dugInfo(HDI), barkedInfo(HBI), AWC(AWC)
{	
	IDA = new IntelligentDigAwarness(this->AWC, this->GI, this->accessInfo, this->dugInfo);
	IMA = new IntelligentMoveAwarness(this->AWC, this->GI, this->dugInfo, this->barkedInfo);
	IEA = new IntelligentEscapeAwarness(this->AWC, this->GI, this->accessInfo);
	ICA = new IntelligentChaisAwarness(this->AWC, this->GI, this->accessInfo);
	IDGA = new IntelligentDogGuardAwarness(this->AWC, this->GI, this->accessInfo);

	this->nextAction = -1;
}

SamuraiManager::~SamuraiManager()
{

}

int SamuraiManager::GetNextAction()
{
	// DEBUG
	//	barkedInfo->DebugPrintMap();
	//	dugInfo->DebugPrintMap();
	//	revealedInfo->DebugPrintMap();
	//	accessInfo->DebugPrintMap(0);
	//	accessInfo->DebugPrintMap(1);
	//	accessInfo->DebugPrintMap(2);
	//	accessInfo->DebugPrintMap(3);
	//	freeInfo->DebugPrintMap();

	//ゲーム進捗率
	// float progressRate = (GI->scores[0] + GI->scores[1])/(digAgnt->TOTAL_TORESURE);
	// float processRate = (GI->step)/(GI->max_step);

	IDA->CreateAwarness();
	IMA->CreateAwarness();
	IEA->CreateAwarness();
	ICA->CreateAwarness();
	IDGA->CreateAwarness();
	nextAction = AWC->DicideAwarness();

	if(nextAction == IntelligentAwarness::SKIP){
		nextAction = -1;
	}

	return this->nextAction;
}