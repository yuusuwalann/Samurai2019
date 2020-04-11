#include "Awarness.h"

Awarness::Awarness(int createStep, ACT_KIND goal_act, Cell goal_value, float AttentionRate, float downRate,IntelligentAwarness* intellAWN):
    createStep(createStep),goal_act(goal_act), goal_value(goal_value), attentionRate(AttentionRate), downRate(downRate), intellAWN(intellAWN){
    error_cnt = 0;
    tmp_data_1 = 0;
    initRate = attentionRate;
}

void Awarness::Update(){
    this->attentionRate = attentionRate - (downRate * 20); //意識減少
    if(this->attentionRate < -160.0F){
        this->attentionRate = -160.0F;
    }
}

Awarness::ACT_KIND Awarness::GetKind(){
    return this->goal_act;
}

int Awarness::GetCreateStep(){
    return this->createStep;
}

float Awarness::GetAttentionRate(){
    return this->attentionRate;
}

float Awarness::GetInitRate(){
    return this->initRate;
}

Cell Awarness::GetGoalValue(){
    return this->goal_value;
}

float Awarness::AddAttentionRate(float addRate){
    this->attentionRate = addRate;
    if(this->attentionRate >= 200.0F){
        this->attentionRate = 200.0F;
    }
    if(this->attentionRate < -160.0F){
        this->attentionRate = -100.0F;
    }

    return this->attentionRate;
}

void Awarness::ChangeGoalValue(const Cell &cell){
    this->goal_value = cell;
}

IntelligentAwarness* Awarness::GetIntellAwa(){
    return this->intellAWN;
}

bool Awarness::operator==(const Awarness & another) const {
	return (goal_act == another.goal_act) && (goal_value == another.goal_value);
}

ostream & operator << (ostream &out, const Awarness &awn) {
    Awarness awn2 = Awarness(awn);
    std::string kindText;
    switch(awn2.GetKind()){
        case Awarness::MOVE:
            kindText = "MOVE";
            break;
        case Awarness::ESCAPE:
            kindText = "ESCAPE";
            break;
        case Awarness::STOKING:
            kindText = "STOKING";
            break;
        case Awarness::NOT_ACT:
            kindText = "NOT_ACT";
            break;
        case Awarness::CHAIS:
            kindText = "CHAIS";
            break;
        case Awarness::DIG:
            kindText = "DIG";
            break;
        case Awarness::SENS:
            kindText = "SENS";
            break;
        case Awarness::GUARD:
            kindText = "GUARD";
            break;
        default:
            kindText = "-";
            break;
    }
	out << "act:" << kindText << " goalCell:" << awn2.GetGoalValue() << "Rate:" << awn2.GetAttentionRate() <<"ErrorCnt:" << awn2.error_cnt << "\n";
	return out;
}
