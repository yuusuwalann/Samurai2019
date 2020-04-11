#include "DogManager.h"

using namespace std;

DogManager::DogManager(AwarnessControlForDog* ACFA, GameInfo* GI, HaveDugInfo* HDI, HaveBarkedInfo* HBI, AccessInfo *ACI, std::vector<GameInfo*>& GIHSTRY)
:ACFA(ACFA), GI(GI), dugInfo(HDI), barkedInfo(HBI), ACI(ACI),GIHSTRY(GIHSTRY) 
{
	ISDA = new IntelligentStokingDogAwarness(this->ACFA, this->GI, this->ACI, GIHSTRY);
	IPDA = new IntelligentPetDogAwarness(this->ACFA, this->GI, this->ACI, this->dugInfo);

}

DogManager::~DogManager()
{
	;
}

int DogManager::GetNextAction(int samuraiMove) {

	ISDA->CreateAwarness();
	IPDA->CreateAwarness();
	nextAction = ACFA->DicideAwarness(samuraiMove);

	if(nextAction == IntelligentAwarness::SKIP){
		nextAction = -1;
	}

	return this->nextAction;
}
