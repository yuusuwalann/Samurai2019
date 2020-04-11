#include "IntelligentChaisAwarness.h"

using namespace std;

IntelligentChaisAwarness::IntelligentChaisAwarness(AwarnessControl* AWC, const GameInfo *GI, AccessInfo *ASI)
    :AWC(AWC),ASI(ASI),IntelligentAwarness(GI){
    ;
}

void IntelligentChaisAwarness::CreateAwarness(){
	//相侍との距離 (自分が負けていたら脱出)
	int eneSamuraiDist = (ASI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_SAMURAI));
	//相犬との距離(脱出)
	int eneDogDist = (ASI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_DOG));

    if (( (eneDogDist < 2) && (eneDogDist >= 0))
        || ( ((eneSamuraiDist < 2) && (eneSamuraiDist >= 0 )) && (GI->scores[ENE_AGENT_SAMURAI] > GI->scores[MY_AGENT_SAMURAI]) ) ){
        if(GI->plans[MY_AGENT_SAMURAI] != GI->actions[MY_AGENT_SAMURAI]){
            //前回と異なる動きをする
            Cell mySamuCell = GI->positions[MY_AGENT_SAMURAI];
            Awarness* aw;
            switch(GI->plans[MY_AGENT_SAMURAI]){
                case (int)M_D:
                case (int)D_D:
                case (int)F_D:
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x - 1, mySamuCell.y))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x - 1, mySamuCell.y), 80.0F, 0.5F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x + 1, mySamuCell.y))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x + 1, mySamuCell.y), 80.0F, 0.5F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x, mySamuCell.y - 1))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x, mySamuCell.y - 1), 80.0F, 0.5F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    break;
                case (int)D_U:
                case (int)M_U:
                case (int)F_U:
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x - 1, mySamuCell.y))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x - 1, mySamuCell.y), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x + 1, mySamuCell.y))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x + 1, mySamuCell.y), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x, mySamuCell.y + 1))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x, mySamuCell.y + 1), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    break;
                case (int)M_R:
                case (int)D_R:
                case (int)F_R:
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x, mySamuCell.y + 1))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x, mySamuCell.y + 1), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x, mySamuCell.y - 1))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x, mySamuCell.y - 1), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x - 1, mySamuCell.y))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x - 1, mySamuCell.y), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    break;
                case (int)M_L:
                case (int)D_L:
                case (int)F_L:
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x, mySamuCell.y + 1))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x, mySamuCell.y + 1), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x, mySamuCell.y - 1))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x, mySamuCell.y - 1), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    if(this->JudgeMoveCell2(Cell(mySamuCell.x + 1, mySamuCell.y))){
                            aw = new Awarness(GI->step, Awarness::CHAIS, Cell(mySamuCell.x + 1, mySamuCell.y), 80.0F, 0.8F, this);
                            aw->tmp_data_1 = 1; //自由領域にerror回数保持
                            if( AWC->ExistAwarnessList(aw) == false) //すでに意識になければ
                                AWC->PushAwarness(aw);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    
    return ;
}

void IntelligentChaisAwarness::UpdateAwarness(Awarness* awn){
	//相侍との距離 (自分が負けていたら脱出)
	int eneSamuraiDist = (ASI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_SAMURAI));
	//相犬との距離(脱出)
	int eneDogDist = (ASI->GetAccessDistance(GI->positions[MY_AGENT_SAMURAI], ENE_AGENT_DOG));

    if (((eneDogDist < 2)&&(eneDogDist >= 0))
        || ( ((eneSamuraiDist < 2)&&(eneSamuraiDist >= 0)) && (GI->scores[ENE_AGENT_SAMURAI] > GI->scores[MY_AGENT_SAMURAI]) ) ){
        if(GI->plans[MY_AGENT_SAMURAI] != GI->actions[MY_AGENT_SAMURAI]){
            awn->tmp_data_1 += 1;
            awn->AddAttentionRate( awn->GetAttentionRate () + (20 * awn->tmp_data_1) );
        }
    }
}

bool IntelligentChaisAwarness::CheckComplate(Awarness* awn){
    if(GI->plans[MY_AGENT_SAMURAI] == GI->actions[MY_AGENT_SAMURAI]){
        return true;
    }
    return false;
}

IntelligentChaisAwarness::Command IntelligentChaisAwarness::JudgeCommand(Awarness* awn){
    return JudgeMoveTargetForSamurai(GI->positions[MY_AGENT_SAMURAI], awn->GetGoalValue());
}