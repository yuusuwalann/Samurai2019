#pragma once

#include "GameInfo.h"
#include "Awarness.h"
#include <queue>

class IntelligentAwarness{
protected:
	const GameInfo *GI;
public:

	enum Command{
	M_D = 0,	//動く
	M_L_D,
	M_L,
	M_L_U,
	M_U,
	M_R_U,
	M_R,
	M_R_D,
	D_D,		//掘る
	D_L_D,
	D_L,
	D_L_U,
	D_U,
	D_R_U,
	D_R,
	D_R_D,
	F_D,		//埋める
	F_L_D,
	F_L,
	F_L_U,
	F_U,
	F_R_U,
	F_R,
	F_R_D,
	SKIP
	};

	static inline int MY_AGENT_SAMURAI;
	static inline int ENE_AGENT_SAMURAI;
	static inline int MY_AGENT_DOG;
	static inline int ENE_AGENT_DOG;
	static inline int TOTAL_TORESURE;

    IntelligentAwarness(const GameInfo* GI);
    virtual void CreateAwarness() = 0;
    virtual void UpdateAwarness(Awarness* awn) = 0;
    virtual bool CheckComplate(Awarness* awn) = 0;
    virtual Command JudgeCommand(Awarness* awn) = 0;
    bool JudgeMoveCell(const Cell& cell);
	bool JudgeMoveCell2(const Cell& cell);
    bool JudgeOutFieldCell(const Cell& cell);
    bool JudgeOutFieldCell2(const Cell& cell);
    bool JudgeHoleCell(const Cell& cell);
    Command JudgeMoveTargetForSamurai(const Cell& target, const Cell& goal);
    Command JudgeMoveTargetForDog(const Cell& target, const Cell& goal);
    bool GetExietSamurai(const Cell& target, int IntelligentAwarnessID);
    bool GetExietDog(const Cell& target, int IntelligentAwarnessID);
    Command GetAgent2AgentDirect(const Cell& from, const Cell& to);
    bool PreActiveSuccess();
};