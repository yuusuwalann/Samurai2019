#include "IntelligentStokingDogAwarness.h"

using namespace std;

IntelligentStokingDogAwarness::IntelligentStokingDogAwarness(AwarnessControlForDog *AWC, const GameInfo *GI, AccessInfo *ASI, std::vector<GameInfo *> &GIHSTRY)
    : AWC(AWC), ASI(ASI), IntelligentAwarness(GI), GIHSTRY(GIHSTRY)
{
    ;
}

//相手のエージェントの予測移動場所へ移動する
void IntelligentStokingDogAwarness::CreateAwarness()
{
    //相手侍 4近傍にもっとも近い位置に向かう
    //すでに4近傍にいる場合は、別の近傍に向かう

    //過去の動き
    int prepreAct;
    if (GI->step > 2)
    {
        prepreAct = GIHSTRY[(GI->step) - 2]->plans[ENE_AGENT_SAMURAI];
    }
    else
    {
        prepreAct = (int)Command::SKIP;
    }

    int preAct;
    if (GI->step > 1)
    {
        preAct = GIHSTRY[(GI->step) - 1]->plans[ENE_AGENT_SAMURAI];
    }
    else
    {
        preAct = (int)Command::SKIP;
    }
    int point[4] = {0, 0, 0, 0}; //0:L 1:R: 2:U 3:D

    //prepre
    switch (prepreAct)
    {
    case (int)Command::M_L:
        point[0] += 2;
        break;
    case (int)Command::M_R:
        point[1] += 2;
        break;
    case (int)Command::M_U:
        point[2] += 2;
        break;
    case (int)Command::M_D:
        point[3] += 2;
        break;
    }

    //pre
    switch (preAct)
    {
    case (int)Command::M_L:
        point[0] += 3;
        break;
    case (int)Command::M_R:
        point[1] += 3;
        break;
    case (int)Command::M_U:
        point[2] += 3;
        break;
    case (int)Command::M_D:
        point[3] += 3;
        break;
    default:
        break;
    }

    //now
    switch (GI->plans[ENE_AGENT_SAMURAI])
    {
    case (int)Command::M_L:
        point[0] += 4;
        break;
    case (int)Command::M_R:
        point[1] += 4;
        break;
    case (int)Command::M_U:
        point[2] += 4;
        break;
    case (int)Command::M_D:
        point[3] += 4;
        break;
    default:
        break;
    }

    //tresure
    {
        Cell targetCell = GI->positions[ENE_AGENT_SAMURAI];
        for (auto tres : GI->revealed_list)
        {
            int dist = ASI->GetAccessDistance(tres.first, MY_AGENT_SAMURAI);
            if(dist < 0){dist = 999; }
            int eSamDis = ASI->GetAccessDistance(tres.first, ENE_AGENT_SAMURAI);
            if(eSamDis < 0){eSamDis = 999; }

            if ((tres.first == Cell(targetCell.x - 1, targetCell.y))||
                (tres.first == Cell(targetCell.x - 2, targetCell.y)))
            {
                if(dist > eSamDis)
                    point[0] += 8;
                else
                    point[0] += 4;
                break;
            }
            else if ((tres.first == Cell(targetCell.x + 1, targetCell.y))||
                (tres.first == Cell(targetCell.x + 2, targetCell.y)))
            {
                if(dist > eSamDis)
                    point[0] += 8;
                else
                    point[0] += 4;
                break;
            }
            else if ((tres.first == Cell(targetCell.x, targetCell.y - 1))||
                (tres.first == Cell(targetCell.x, targetCell.y - 2)))
            {
                if(dist > eSamDis)
                    point[0] += 8;
                else
                    point[0] += 4;
                break;
            }
            else if ((tres.first == Cell(targetCell.x, targetCell.y + 1))||
                (tres.first == Cell(targetCell.x, targetCell.y + 2)))
            {
                if(dist > eSamDis)
                    point[0] += 8;
                else
                    point[0] += 4;
                break;
            }
            else
            {
                ;
            }
        }
    }

    //distance
    {
        Command direct = GetAgent2AgentDirect(GI->positions[MY_AGENT_DOG], GI->positions[ENE_AGENT_SAMURAI]);
        switch(direct){
            case Command::M_D:
                point[3] += 8;
                break;
            case Command::M_L_D:
                point[0] += 4;
                point[3] += 4;
                break;
            case Command::M_R_D:
                point[1] += 4;
                point[3] += 4;
                break;
            case Command::M_U:
                point[2] += 8;
                break;
            case Command::M_L_U:
                point[0] += 4;
                point[2] += 4;
                break;
            case Command::M_R_U:
                point[1] += 4;
                point[2] += 4;
                break;
            case Command::M_R:
                point[1] += 8;
                break;
            case Command::M_L:
                point[0] += 8;
                break;
            default:
                break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        Cell targetCell = GI->positions[ENE_AGENT_SAMURAI];
        switch (i)
        {
        case 0: //左?
            targetCell = Cell(targetCell.x - 1, targetCell.y);
            break;
        case 1: //右?
            targetCell = Cell(targetCell.x + 1, targetCell.y);
            break;
        case 2: //上
            targetCell = Cell(targetCell.x, targetCell.y - 1);
            break;
        case 3: //下
            targetCell = Cell(targetCell.x, targetCell.y + 1);
            break;
        default:
            break;
        }
        if (JudgeOutFieldCell(targetCell) == false)
        {
            continue;
        }
        Awarness *aw = new Awarness(GI->step, Awarness::STOKING, targetCell, 20.0F + 10.0F * point[i], 0.5F, this);
        AWC->PushAwarness(aw);
    }

    return;
}

void IntelligentStokingDogAwarness::UpdateAwarness(Awarness *awn)
{
    ;
}

bool IntelligentStokingDogAwarness::CheckComplate(Awarness *awn)
{
    if (awn->GetCreateStep() != GI->step)
    {
        return true; //常に新しく更新していく
    }
    else
    {
        return false;
    }
}

IntelligentStokingDogAwarness::Command IntelligentStokingDogAwarness::JudgeCommand(Awarness *awn)
{
    Command cmd = SKIP;
    if (GI->positions[MY_AGENT_DOG] == awn->GetGoalValue())
    { //すでに接近済みであるなら
        cmd = SKIP;
    }
    else
    {
        cmd = JudgeMoveTargetForDog(GI->positions[MY_AGENT_DOG], awn->GetGoalValue());
    }

    return cmd;
}