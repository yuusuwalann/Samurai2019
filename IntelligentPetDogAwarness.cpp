#include "IntelligentPetDogAwarness.h"

using namespace std;

IntelligentPetDogAwarness::IntelligentPetDogAwarness(AwarnessControlForDog *AWC, const GameInfo *GI, AccessInfo *ASI, HaveDugInfo* HDI)
    : AWC(AWC), ASI(ASI), IntelligentAwarness(GI), HDI(HDI)
{
    ;
}

//自分の侍のお供をする
void IntelligentPetDogAwarness::CreateAwarness()
{
    //把握している財宝を知らせる
    //自侍との距離が近く、対象セルが相手侍より遠い
    for(auto tre : GI->sensed_list){
        int dist = ASI->GetAccessDistance(tre.first, MY_AGENT_SAMURAI);
        int eSamDis = ASI->GetAccessDistance(tre.first, ENE_AGENT_SAMURAI);
        if(eSamDis < 0){eSamDis = 999; }
        int eDogDis = ASI->GetAccessDistance(tre.first, ENE_AGENT_DOG);
        if(eDogDis < 0){eDogDis = 999; }
        float attentionTemp = 0.0F;

        //財宝の量と勝利までの閾値による意識
        if (tre.second + GI->scores[MY_AGENT_SAMURAI] >= TOTAL_TORESURE / 2){
            //勝利確定財宝量
            attentionTemp += 50.0F;
        }else if(tre.second >= TOTAL_TORESURE / 10){
            //ゲーム上比重高い財宝量
            attentionTemp += 30.0F;
        }else{
            //あまり重要でない財宝
            attentionTemp += 30.0F;
        }

        //財宝までの距離への意識
        if((dist < 3) && ( dist >= 0)){
            if( (min(eSamDis, eDogDis) - (dist + 1)) > 0) //邪魔なエージェントがいない
                //すぐに獲得できる財宝
                attentionTemp += 50.0F;
            else
                //危険性のある財宝
                attentionTemp += 30.0F;
        }else if((dist < 5) && ( dist >= 0)){
            if( (min(eSamDis, eDogDis) - (dist + 1)) > 0) //邪魔なエージェントがいない
                //すぐに獲得できる財宝
                attentionTemp += 40.0F;
            else
                //危険性のある財宝
                attentionTemp += 20.0F;
        }else if((dist < GI->size/2) && ( dist >= 0)){
            if( (min(eSamDis, eDogDis) - (dist + 1)) > 0) //邪魔なエージェントがいない
                //すぐに獲得できる財宝
                attentionTemp += 30.0F;
            else
                //危険性のある財宝
                attentionTemp += 10.0F;
        }else if(dist >= 0){
            if( (min(eSamDis, eDogDis) - (dist + 1)) > 0) //邪魔なエージェントがいない
                //すぐに獲得できる財宝
                attentionTemp += 10.0F;
            else
                //危険性のある財宝
                attentionTemp += 0.0F;
        }else{
            attentionTemp += 0.0;
        }

        Awarness* aw = new Awarness(GI->step, Awarness::SENS, tre.first, attentionTemp, 0.5F, this );
        if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
            AWC->PushAwarness(aw);
    }

}

void IntelligentPetDogAwarness::UpdateAwarness(Awarness *awn)
{
    int dist = ASI->GetAccessDistance(awn->GetGoalValue(), MY_AGENT_SAMURAI);
    int eSamDis = ASI->GetAccessDistance(awn->GetGoalValue(), ENE_AGENT_SAMURAI);
    if(eSamDis < 0){eSamDis = 999; }
    int eDogDis = ASI->GetAccessDistance(awn->GetGoalValue(), ENE_AGENT_DOG);
    if(eDogDis < 0){eDogDis = 999; }

    //財宝までの距離への意識
    if((dist < 3) && ( dist >= 0)){
        if( (min(eSamDis, eDogDis) - (dist + 1)) > 0) //邪魔なエージェントがいない
            //すぐに獲得できる財宝
            awn->AddAttentionRate(awn->GetAttentionRate() + 30.0F);
    }else if((dist < 5) && ( dist >= 0)){
        if( (min(eSamDis, eDogDis) - (dist + 1)) > 0) //邪魔なエージェントがいない
            //すぐに獲得できる財宝
            awn->AddAttentionRate(awn->GetAttentionRate() + 20.0F);
    }else{
        ;
    }
}

bool IntelligentPetDogAwarness::CheckComplate(Awarness *awn)
{
    //掘られたか
    if(HDI->GetDugInfo(awn->GetGoalValue())){
        return true;
    }

    //公開されたか
    for(auto itr : GI->revealed_list){
        if(awn->GetGoalValue() == itr.first){
            return true;
        }
    }

    return false;
}

IntelligentPetDogAwarness::Command IntelligentPetDogAwarness::JudgeCommand(Awarness *awn)
{
    return JudgeMoveTargetForDog(GI->positions[MY_AGENT_DOG], awn->GetGoalValue());
}