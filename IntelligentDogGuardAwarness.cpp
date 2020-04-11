#include "IntelligentDogGuardAwarness.h"

using namespace std;

struct SuperCell : public Cell{
    public:
        int depth;
        SuperCell(Cell cell, int depth):Cell(cell),depth(depth){;}
        ~SuperCell(){}
};

IntelligentDogGuardAwarness::IntelligentDogGuardAwarness(AwarnessControl* AWC, const GameInfo *GI, AccessInfo *ASI)
    :AWC(AWC),ASI(ASI),IntelligentAwarness(GI){
    ;
}

void IntelligentDogGuardAwarness::CreateAwarness(){
    //4近傍をチェックする
    //仮にそこに穴を開けた場合に自侍と敵犬の距離が増大または到達不能になれば、
    //相手の犬との距離が3以上離れているものとする
    //その行動を意識する

    if(GI->step >= (GI->max_step) - 2){ //最終局面は除外
        return ;
    }

    if(ASI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_DOG) >= 3){ //すでに到達不可なら考えない
        for(int pattern =0; pattern < 4; pattern++){
            //堀先がセル上存在していて、掘ることが可能な場合
            Cell targetCell;
            switch(pattern){
                case 0:
                    targetCell = Cell(GI->positions[MY_AGENT_SAMURAI].x + 1, GI->positions[MY_AGENT_SAMURAI].y);
                    break;
                case 1:
                    targetCell = Cell(GI->positions[MY_AGENT_SAMURAI].x - 1, GI->positions[MY_AGENT_SAMURAI].y);
                    break;
                case 2:
                    targetCell = Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y + 1);
                    break;
                case 3:
                    targetCell = Cell(GI->positions[MY_AGENT_SAMURAI].x, GI->positions[MY_AGENT_SAMURAI].y - 1);
                    break;
                default:
                    break;
            }
            if(JudgeMoveCell(targetCell) ==  false){ //掘れない
                continue;
            }
            if(JudgeHoleCell(targetCell) ==  true){ //すでに穴
                continue;
            }

            //掘った過程で距離算出
            map<Cell, int> newDogAIM;
            for (int i = 0; i < GI->size; i++)
            {
                for (int j = 0; j < GI->size; j++)
                {
                    newDogAIM[Cell(i, j)] = INIT_INFO_VALUE;
                }
            }

            //AccessMAP生成
            queue<SuperCell> que;
            que.push(SuperCell(GI->positions[ENE_AGENT_DOG], 0));
            newDogAIM[GI->positions[ENE_AGENT_DOG]] = 0;
            while (que.size() > 0)
            {
                SuperCell popCell = que.front();
                que.pop();
                for (int j = 0; j < 8; j++)
                {
                    Cell tmpCell;
                    switch (j)
                    {
                    case (0): //Up
                        tmpCell = Cell(popCell.x, popCell.y - 1);
                        break;
                    case (1): //Down
                        tmpCell = Cell(popCell.x, popCell.y + 1);
                        break;
                    case (2): //Left
                        tmpCell = Cell(popCell.x - 1, popCell.y);
                        break;
                    case (3): //Right
                        tmpCell = Cell(popCell.x + 1, popCell.y);
                        break;
                    case (4): //Up_Left
                        tmpCell = Cell(popCell.x - 1, popCell.y - 1);
                        break;
                    case (5): //Down_Left
                        tmpCell = Cell(popCell.x - 1, popCell.y + 1);
                        break;
                    case (6): //Up_Right
                        tmpCell = Cell(popCell.x + 1, popCell.y - 1);
                        break;
                    case (7): //Down_Right
                        tmpCell = Cell(popCell.x + 1, popCell.y - 1);
                        break;
                    default:
                        break;
                    }

                    if ((JudgeOutFieldCell(tmpCell))&&(JudgeHoleCell(tmpCell) == false))
                    { //枠内&穴でない(犬は穴だめ)
                        if((tmpCell == targetCell) == false){ //今回の試し掘りも穴とする
                            if (newDogAIM[tmpCell] == INIT_INFO_VALUE)
                            { //未設定
                                que.push(SuperCell(tmpCell, popCell.depth + 1));
                                newDogAIM[tmpCell] = popCell.depth + 1;
                            }
                        }
                    }
                }
            }

            //距離の増減チェック
            int prevDist = ASI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_DOG);
            int retDist = newDogAIM[GI->positions[MY_AGENT_SAMURAI]];
            if( (retDist - prevDist > 3) || (retDist == INIT_INFO_VALUE) ){ //距離が3以上か到達不可であったら
                Awarness* aw = new Awarness(GI->step, Awarness::GUARD, targetCell, 30.0, 0.9F, this );
                if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                    AWC->PushAwarness(aw);
            }
        }
    }
}

void IntelligentDogGuardAwarness::UpdateAwarness(Awarness* awn){
    ; //1ターンしか生存しない
}

bool IntelligentDogGuardAwarness::CheckComplate(Awarness* awn){
    //ターンが2ターン経過してたら削除(ループ回避)
    if( (GI->step -1 ) > awn->GetCreateStep()){
        return true;
    }else{
        return false;
    }
}

IntelligentDogGuardAwarness::Command IntelligentDogGuardAwarness::JudgeCommand(Awarness* awn){
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
    return SKIP;
}