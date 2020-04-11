// SamuraiCoding.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <vector>
#include "GameInfo.h"
#include "SamuraiManager.h"
#include "DogManager.h"
#include "AccessInfo.h"
#include "RevealedMapInfo.h"
#include "MoveFreeInfo.h"
#include "HaveDugInfo.h"
#include "HaveBarkedInfo.h"
#include "AwarnessControl.h"
#include "AwarnessControlForDog.h"

using namespace std;

int main()
{
	bool initFlg = true;
	//永続メモリ
	AccessInfo* ACI;
	RevealedMapInfo* RMI;
	MoveFreeInfo* MFI;
	HaveDugInfo* HDI;
	HaveBarkedInfo* HBI;
	AwarnessControl* AWC;
	AwarnessControlForDog* AWCFD;
    std::vector<GameInfo*> GIHSTY;

	while (true) {
		GameInfo GI = GameInfo(cin);
		GIHSTY.push_back(new GameInfo(GI));
		if(initFlg){
			initFlg = false;
			ACI = new AccessInfo(&GI);
			RMI = new RevealedMapInfo(&GI);
			MFI = new MoveFreeInfo(&GI);
			HDI = new HaveDugInfo(&GI);
			HBI = new HaveBarkedInfo(&GI);
			AWC = new AwarnessControl(&GI);
			AWCFD = new AwarnessControlForDog(&GI);
			ACI->Update(&GI);
			RMI->Update(&GI);
			MFI->Update(&GI);
			HDI->Update(&GI);
			HBI->Update(&GI);
			AWC->Update(&GI);
			AWCFD->Update(&GI);
		}else{
			ACI->Update(&GI);
			RMI->Update(&GI);
			MFI->Update(&GI);
			HDI->Update(&GI);
			HBI->Update(&GI);
			AWC->Update(&GI);
			AWCFD->Update(&GI);
		}
		if (GI.id <= 1) {
			SamuraiManager* pSamMng = new SamuraiManager(&GI, ACI, RMI,MFI, HDI, HBI, AWC);
			cout << pSamMng->GetNextAction() << endl;
			delete(pSamMng);
		}
		else {
			SamuraiManager* pSamMng = new SamuraiManager(&GI, ACI, RMI,MFI, HDI, HBI, AWC);
			DogManager* pDogMng = new DogManager(AWCFD, &GI, HDI, HBI, ACI, GIHSTY);
			cout << pDogMng->GetNextAction(pSamMng->GetNextAction()) << endl;
			delete(pDogMng);
		}
	}
}
