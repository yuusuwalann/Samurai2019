#include "IntelligentDigAwarness.h"

IntelligentDigAwarness::IntelligentDigAwarness(AwarnessControl* AWC, const GameInfo *GI, AccessInfo *ASI, HaveDugInfo* HDI)
    :AWC(AWC),ASI(ASI),HDI(HDI),IntelligentAwarness(GI){
    ;
}

void IntelligentDigAwarness::CreateAwarness(){

    //最後の埋蔵金であればその財宝に意識集中
    if (GI->revealed_num == 1)
    {
        if (TOTAL_TORESURE == (GI->scores[MY_AGENT_SAMURAI] + GI->scores[ENE_AGENT_SAMURAI] + GI->revealed_list.begin()->second))
        {
            Awarness* aw = new Awarness(GI->step, Awarness::DIG, GI->revealed_list.begin()->first, Awarness::ATTENTION_STRONG, 0.1F, this );
            
            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                AWC->PushAwarness(aw);
            return;
        }
    }

    //財宝距離のトータル算出
    int totalDistant = 0;
    for (auto itr : GI->revealed_list)
    {
        int tmpdist = ASI->GetAccessDistance(itr.first, MY_AGENT_SAMURAI);
        if(tmpdist >= 0){
            totalDistant += tmpdist;
        }
    }
    
    //すべての財宝について意識する
    for (auto itr : GI->revealed_list)
    {
        int dist = ASI->GetAccessDistance(itr.first, MY_AGENT_SAMURAI);
        int eSamDis = ASI->GetAccessDistance(itr.first, ENE_AGENT_SAMURAI);
        if(eSamDis < 0) { eSamDis = 999; }
        int eDogDis = ASI->GetAccessDistance(itr.first, ENE_AGENT_DOG);
        if(eDogDis < 0) { eDogDis = 999; }

        float attentionTemp = 0.0F;

        //財宝の量と勝利までの閾値による意識
        if (itr.second + GI->scores[MY_AGENT_SAMURAI] >= TOTAL_TORESURE / 2){
            //勝利確定財宝量
            attentionTemp += 50.0F;
        }else if(itr.second >= TOTAL_TORESURE / 10){
            //ゲーム上比重高い財宝量
            attentionTemp += 30.0F;
        }else{
            //あまり重要でない財宝
            attentionTemp += 30.0F;
        }

        //財宝までの距離への意識
        if((dist < 3) && (dist >= 0)){
            if( (min(eSamDis, eDogDis) - (dist)) > 0){ //邪魔なエージェントがいない
                //すぐに獲得できる財宝
                attentionTemp += 70.0F;
            }
        }else if((dist < 5) && (dist >= 0)){
            if( (min(eSamDis, eDogDis) - (dist)) > 0){ //邪魔なエージェントがいない
                //すぐに獲得できる財宝
                attentionTemp += 50.0F;
            }
        }

        //相対的な距離による加算
        if(totalDistant != 0){
            if(dist >= 0){
                float ans = 1 - ((float)dist / (float)totalDistant);
                attentionTemp += ans*10.0F;
            }
        }

        //同財宝近くも財宝がある場合加算
        for (auto itr2 : GI->revealed_list)
        {
            if(itr.first == itr2.first){
                continue;
            }else{
                if( (itr2.first == Cell(itr.first.x + 1, itr.first.y)) ||
                    (itr2.first == Cell(itr.first.x - 1, itr.first.y)) ||
                    (itr2.first == Cell(itr.first.x, itr.first.y + 1)) ||
                    (itr2.first == Cell(itr.first.x, itr.first.y - 1)) ||
                    (itr2.first == Cell(itr.first.x + 1, itr.first.y + 1)) ||
                    (itr2.first == Cell(itr.first.x + 1, itr.first.y - 1)) ||
                    (itr2.first == Cell(itr.first.x - 1, itr.first.y + 1)) ||
                    (itr2.first == Cell(itr.first.x - 1, itr.first.y - 1)) ){
                        attentionTemp += itr2.second / 10;
                }
            }
        }

        Awarness* aw = new Awarness(GI->step, Awarness::DIG, itr.first, attentionTemp, 0.1F, this );
        if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
            AWC->PushAwarness(aw);
    }

    return ;
}

void IntelligentDigAwarness::UpdateAwarness(Awarness* awn){
    int dist = ASI->GetAccessDistance(awn->GetGoalValue(), MY_AGENT_SAMURAI);
    int eSamDis = ASI->GetAccessDistance(awn->GetGoalValue(), ENE_AGENT_SAMURAI);
    if(eSamDis < 0) { eSamDis = 999; }
    int eDogDis = ASI->GetAccessDistance(awn->GetGoalValue(), ENE_AGENT_DOG);
    if(eDogDis < 0) { eDogDis = 999; }

    //距離推定

    //財宝までの距離への意識
    if((dist < 3) && (dist >= 0)){
        if( (min(eSamDis, eDogDis) - (dist)) >= 0){ //邪魔なエージェントがいない
            //すぐに獲得できる財宝
            awn->AddAttentionRate( ( awn->GetAttentionRate() < 0 ? 100: awn->GetAttentionRate() + 70 ) );
            if( (min(eSamDis, eDogDis) - (dist + 3)) >= 0){
                awn->error_cnt = 0; //errorカウントクリア
            }
        }
    }else if((dist < 5) && (dist >= 0)){
        if( (min(eSamDis, eDogDis) - (dist)) >= 0){ //邪魔なエージェントがいない
            //すぐに獲得できる財宝
            awn->AddAttentionRate(( awn->GetAttentionRate() < 0 ? 70: awn->GetAttentionRate() + 50 ));
            if( (min(eSamDis, eDogDis) - (dist + 3)) >= 0){
                awn->error_cnt = 0; //errorカウントクリア
            }
        }
    }else if((dist < 7) && (dist >= 0)){
        if( (min(eSamDis, eDogDis) - (dist)) >= 0){ //邪魔なエージェントがいない
            //すぐに獲得できる財宝
            awn->AddAttentionRate(( awn->GetAttentionRate() < 0 ? 50: awn->GetAttentionRate() + 30 ));
            if( (min(eSamDis, eDogDis) - (dist + 3)) >= 0){
                awn->error_cnt = 0; //errorカウントクリア
            }
        }
    }else if((dist < 11) && (dist >= 0)){
        if( (min(eSamDis, eDogDis) - (dist)) >= 0){ //邪魔なエージェントがいない
            //すぐに獲得できる財宝
            awn->AddAttentionRate(( awn->GetAttentionRate() < 0 ? 30: awn->GetAttentionRate() + 10 ));
            if( (min(eSamDis, eDogDis) - (dist + 3)) >= 0){
                awn->error_cnt = 0; //errorカウントクリア
            }
        }
    }else{
        ;
    }

    //エラー回避
    if(awn->error_cnt >= 5)
        awn->AddAttentionRate( awn->GetAttentionRate() - 20.0F);
    if(awn->error_cnt >= 10)
        awn->AddAttentionRate( awn->GetAttentionRate() - 20.0F);
}

bool IntelligentDigAwarness::CheckComplate(Awarness* awn){
    if(HDI->GetDugInfo(awn->GetGoalValue())){
        return true;
    }
    return false;
}

IntelligentDigAwarness::Command IntelligentDigAwarness::JudgeCommand(Awarness* awn){
    //4近傍に入っていれば掘る
    if(GetExietSamurai(awn->GetGoalValue(), MY_AGENT_SAMURAI)){
        if(JudgeMoveCell(awn->GetGoalValue())){
            Command cmd = GetAgent2AgentDirect(GI->positions[MY_AGENT_SAMURAI], awn->GetGoalValue());
            if(cmd == M_U) { return D_U;}
            if(cmd == M_D) { return D_D;}
            if(cmd == M_L) { return D_L;}
            if(cmd == M_R) { return D_R;}
        }
    }
    //そうでなければ移動
    return JudgeMoveTargetForSamurai(GI->positions[MY_AGENT_SAMURAI], awn->GetGoalValue());
}
