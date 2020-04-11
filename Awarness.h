#pragma once

#include "Cell.h"
#include "GameInfo.h"

class IntelligentAwarness;
class Awarness{
public:
	enum ACT_KIND{
	NOT_ACT = 0,	//動く
    DIG,
    MOVE,
    ESCAPE,
    STOKING,
    SENS,
    CHAIS,
    GUARD
    };
    
private:
    int createStep; //意識が生成されたstep
    ACT_KIND goal_act; //掘る, 移動
    Cell goal_value; //埋蔵金セル or 移動目標セル
    float attentionRate; //意識度
    float initRate; //初期意識度
    float downRate; //意識の減少度
    IntelligentAwarness* intellAWN;

public:
    int tmp_data_1; //自由に使えるデータ領域
    int error_cnt;

    static constexpr float ATTENTION_STRONG = 100.0F;
    static constexpr float ATTENTION_NORMAL = 80.0F;

    Awarness(int createStep, ACT_KIND goal_act, Cell goal_value, float attentionRate, float downRate,IntelligentAwarness* intellAWN);
    void Update();

    float GetAttentionRate();
    int GetCreateStep();
    ACT_KIND GetKind();
    Cell GetGoalValue();
    float GetInitRate();
    float AddAttentionRate(float addRate);
    void ChangeGoalValue(const Cell &cell);
    IntelligentAwarness* GetIntellAwa();
	bool operator==(const Awarness &another) const;
};

ostream & operator << (ostream &out, const Awarness &awn);