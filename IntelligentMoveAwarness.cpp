#include "IntelligentMoveAwarness.h"

IntelligentMoveAwarness::IntelligentMoveAwarness(AwarnessControl *AWC, const GameInfo *GI, HaveDugInfo *HDI, HaveBarkedInfo *HBI)
    : AWC(AWC), HDI(HDI), HBI(HBI), IntelligentAwarness(GI)
{
    ;
}

void IntelligentMoveAwarness::CreateAwarness()
{
    //Depthマス同一方向に動いた場合にたくさん掘れる方向に向かう
    float digRate[4];
    for (int i = 0; i < 4; i++)
    {
        Cell tmp = GI->positions[MY_AGENT_SAMURAI];
        Cell target = tmp;
        digRate[i] = 0.0F;

        //先読みしてたくさん掘れる方向を決定する
        for (int j = 0; j < DEPTH_SIZE; j++)
        {
            float f_tmp;
            switch (i)
            {
            case 0: //up
                target = Cell(target.x, target.y - 1);
                f_tmp = GetPossibleDigRate(target, Command::M_U);
                digRate[i] = (digRate[i] + f_tmp) / 2;
                break;
            case 1: //down
                target = Cell(target.x, target.y + 1);
                f_tmp = GetPossibleDigRate(target, Command::M_D);
                digRate[i] = (digRate[i] + f_tmp) / 2;
                break;
            case 2: //right
                target = Cell(target.x + 1, target.y);
                f_tmp = GetPossibleDigRate(target, Command::M_R);
                digRate[i] = (digRate[i] + f_tmp) / 2;
                break;
            case 3: //left
                target = Cell(target.x - 1, target.y);
                f_tmp = GetPossibleDigRate(target, Command::M_L);
                digRate[i] = (digRate[i] + f_tmp) / 2;
                break;
            default:
                break;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        float attentionTemp;
        if(digRate[i] > 0.5F){
            attentionTemp = 30.0F;
        }else if(digRate[i] > 0.3F){
            attentionTemp = 20.0F;
        }else{
            attentionTemp = 0.0F;
        }

        Cell target;
        switch (i)
        {
        case 0:
            target = Cell(GI->positions[MY_AGENT_SAMURAI].x, max(GI->positions[MY_AGENT_SAMURAI].y - DEPTH_SIZE, 1));
            break;
        case 1:
            target = Cell(GI->positions[MY_AGENT_SAMURAI].x, min(GI->positions[MY_AGENT_SAMURAI].y + DEPTH_SIZE, GI->size - 2));
            break;
        case 2:
            target = Cell(min(GI->positions[MY_AGENT_SAMURAI].x + DEPTH_SIZE, GI->size -2), GI->positions[MY_AGENT_SAMURAI].y);
            break;
        case 3:
            target = Cell(max(GI->positions[MY_AGENT_SAMURAI].x - DEPTH_SIZE, 1), GI->positions[MY_AGENT_SAMURAI].y);
            break;
        default:
            break;
        }

        Awarness* aw = new Awarness(GI->step, Awarness::MOVE, target, attentionTemp, 0.8F, this);

        if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
            AWC->PushAwarness(aw);
    }
}

float IntelligentMoveAwarness::GetPossibleDigRate(const Cell &targetCell, Command nextDirect)
{
    int retCnt = 0;
    int baseCnt = 0;
    if ((nextDirect != Command::M_U) && (nextDirect != Command::F_U))
    {
        if (JudgeOutFieldCell(Cell(targetCell.x, targetCell.y - 1)))
        {
            baseCnt++;
            if (HDI->GetDugInfo(Cell(targetCell.x, targetCell.y - 1)) == false)
                retCnt++;
        }
    }
    if ((nextDirect != Command::M_D) && (nextDirect != Command::F_D))
    {
        if (JudgeOutFieldCell(Cell(targetCell.x, targetCell.y + 1)))
        {
            baseCnt++;
            if (HDI->GetDugInfo(Cell(targetCell.x, targetCell.y + 1)) == false)
                retCnt++;
        }
    }
    if ((nextDirect != Command::M_R) && (nextDirect != Command::F_R))
    {
        if (JudgeOutFieldCell(Cell(targetCell.x + 1, targetCell.y)))
        {
            baseCnt++;
            if (HDI->GetDugInfo(Cell(targetCell.x + 1, targetCell.y)) == false)
                retCnt++;
        }
    }
    if ((nextDirect != Command::M_L) && (nextDirect != Command::F_L))
    {
        if (JudgeOutFieldCell(Cell(targetCell.x - 1, targetCell.y)))
        {
            baseCnt++;
            if (HDI->GetDugInfo(Cell(targetCell.x - 1, targetCell.y)) == false)
                retCnt++;
        }
    }
    return (float)retCnt / (float)baseCnt;
}

void IntelligentMoveAwarness::UpdateAwarness(Awarness* awn){
    ;
}

bool IntelligentMoveAwarness::CheckComplate(Awarness* awn){
    if(GI->positions[MY_AGENT_SAMURAI] == awn->GetGoalValue()){
        return true;
    }
    return false;
}

IntelligentMoveAwarness::Command IntelligentMoveAwarness::JudgeCommand(Awarness* awn){
    //目標の地点まで掘りながら進む(掘り優先)
    //進むべき方向以外を掘る
    Command nextDirect = JudgeMoveTargetForSamurai(GI->positions[MY_AGENT_SAMURAI],awn->GetGoalValue());
    //cout << "JudgeMoveTargetForSamurai:" << nextDirect << endl;
    if((nextDirect != Command::M_U) && (nextDirect != Command::F_U)){
        if(JudgeMoveCell(Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y -1)))
            if(HDI->GetDugInfo(Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y -1)) == false){
                if(HBI->GetBarkedInfo(Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y -1)) == false){
                    return Command::D_U;
                }
            }
    }
    if((nextDirect != Command::M_D) && (nextDirect != Command::F_D)){
        if(JudgeMoveCell(Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y +1)))
            if(HDI->GetDugInfo(Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y +1)) == false){
                if(HBI->GetBarkedInfo(Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y +1)) == false){
                    return Command::D_D;
                }
            }
    }
    if((nextDirect != Command::M_R) && (nextDirect != Command::F_R)){
        if(JudgeMoveCell(Cell(GI->positions[MY_AGENT_SAMURAI].x + 1, GI->positions[MY_AGENT_SAMURAI].y)))
            if(HDI->GetDugInfo(Cell(GI->positions[MY_AGENT_SAMURAI].x + 1, GI->positions[MY_AGENT_SAMURAI].y)) == false){
                if(HBI->GetBarkedInfo(Cell(GI->positions[MY_AGENT_SAMURAI].x + 1, GI->positions[MY_AGENT_SAMURAI].y)) == false){
                    return Command::D_R;
                }
            }
    }
    if((nextDirect != Command::M_L) && (nextDirect != Command::F_L)){
        if(JudgeMoveCell(Cell(GI->positions[MY_AGENT_SAMURAI].x - 1, GI->positions[MY_AGENT_SAMURAI].y)))
            if(HDI->GetDugInfo(Cell(GI->positions[MY_AGENT_SAMURAI].x - 1, GI->positions[MY_AGENT_SAMURAI].y)) == false){
                if(HBI->GetBarkedInfo(Cell(GI->positions[MY_AGENT_SAMURAI].x - 1, GI->positions[MY_AGENT_SAMURAI].y)) == false){
                    return Command::D_L;
                }
            }
    }

    //最後の一歩が四隅なら実行しない
    switch(nextDirect){
        case(Command::M_D):
            if (JudgeMoveCell2(Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y + 1)) == false){
                return Command::SKIP;
            }
            break;
        case(Command::M_L):
            if (JudgeMoveCell2(Cell(GI->positions[MY_AGENT_SAMURAI].x - 1, GI->positions[MY_AGENT_SAMURAI].y)) == false){
                return Command::SKIP;
            }
            break;
        case(Command::M_U):
            if(JudgeMoveCell2(Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y - 1)) == false){
                return Command::SKIP;
            }
            break;
        case(Command::M_R):
            if(JudgeMoveCell2(Cell(GI->positions[MY_AGENT_SAMURAI].x + 1, GI->positions[MY_AGENT_SAMURAI].y)) == false){
                return Command::SKIP;
            }
            break;
        default:
            break;
    }

    return nextDirect;
}