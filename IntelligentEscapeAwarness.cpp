#include "IntelligentEscapeAwarness.h"

using namespace std;

IntelligentEscapeAwarness::IntelligentEscapeAwarness(AwarnessControl* AWC, const GameInfo *GI, AccessInfo *ASI)
    :AWC(AWC),ASI(ASI),IntelligentAwarness(GI){
    ;
}

void IntelligentEscapeAwarness::CreateAwarness(){
    float attentionTemp = 0.0F;
 	//自犬との距離
	// int myDogdist = (AI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], MY_AGENT_DOG));
	//相侍との距離
	// int eneSamuraiDist = (AI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_SAMURAI));
	//相犬との距離
	int eneDogDist = (ASI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_DOG));

    if(eneDogDist == -1){   //到達不可能地点に居れば意識しない
        return ;
    }

    if ((eneDogDist < 3) && (eneDogDist >= 0)){
        attentionTemp = 20.0F; 
    }else if((eneDogDist < 5) && (eneDogDist >= 0)){
        attentionTemp = 10.0F;
    }else if((eneDogDist < 10) && (eneDogDist >= 0)){
        attentionTemp = 5.0F;
    }else{
        return;
    }

    Command tmpCmd = GetAgent2AgentDirect(GI->positions[ENE_AGENT_DOG], GI->positions[MY_AGENT_SAMURAI]);

    Cell target;
    switch (tmpCmd)
    {
        case M_U:
            target = Cell(GI->positions[MY_AGENT_SAMURAI].x, max(GI->positions[MY_AGENT_SAMURAI].y - 5, 1));
            break;
        case M_D:
            target = Cell(GI->positions[MY_AGENT_SAMURAI].x, min(GI->positions[MY_AGENT_SAMURAI].y + 5, GI->size - 2));
            break;
        case M_R:
        case M_R_D:
        case M_R_U:
            target = Cell(min(GI->positions[MY_AGENT_SAMURAI].x + 5, GI->size -2), GI->positions[MY_AGENT_SAMURAI].y);
            break;
        case M_L:
        case M_L_D:
        case M_L_U:
            target = Cell(max(GI->positions[MY_AGENT_SAMURAI].x - 5, 1), GI->positions[MY_AGENT_SAMURAI].y);
            break;
        default:
            return ;
    }
    
    Awarness* aw = new Awarness(GI->step, Awarness::ESCAPE, target, attentionTemp, 0.5F, this);
    aw->tmp_data_1 = eneDogDist; //自由領域に保持

    if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
        AWC->PushAwarness(aw);
    
    return ;
}

void IntelligentEscapeAwarness::UpdateAwarness(Awarness* awn){
    int eneDogDist = (ASI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_DOG));
    if(eneDogDist != -1){
        if((eneDogDist + 1) <= awn->tmp_data_1){   //前回の犬との位置とを比較(犬の移動速度補正あり)
            //犬が前回より近づいている。
            awn->AddAttentionRate(awn->GetAttentionRate() - 5.0F); //現在の行動は失策
        }
    }
}

bool IntelligentEscapeAwarness::CheckComplate(Awarness* awn){
    if(GI->positions[MY_AGENT_SAMURAI] == awn->GetGoalValue()){
        return true;
    }
    return false;
}

IntelligentEscapeAwarness::Command IntelligentEscapeAwarness::JudgeCommand(Awarness* awn){
    return JudgeMoveTargetForSamurai(GI->positions[MY_AGENT_SAMURAI], awn->GetGoalValue());
}