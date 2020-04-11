#include "IntelligentAwarness.h"

using namespace std;

class SuperCell : public Cell{
public:
    IntelligentAwarness::Command direction;
    SuperCell(int x, int y, IntelligentAwarness::Command direction)
        :Cell(x, y), direction(direction){;}
    ~SuperCell(){;}
};

IntelligentAwarness::IntelligentAwarness(const GameInfo* GI):GI(GI){
    MY_AGENT_SAMURAI = (GI->id % 2) == 0 ? 0 : 1;
    ENE_AGENT_SAMURAI = (GI->id %2 ) == 0 ? 1 : 0;
    MY_AGENT_DOG = (GI->id %2) == 0 ? 2 : 3;
    ENE_AGENT_DOG = (GI->id %2) == 0 ? 3 : 2;
    //total埋蔵金算出
	TOTAL_TORESURE = GI->scores[0] + GI->scores[1] + GI->remaining;
	for(auto tra : GI->revealed_list) { TOTAL_TORESURE += tra.second; }
}

bool IntelligentAwarness::JudgeMoveCell(const Cell& cell){
    //フィールド外は無効
    if((cell.y < 0) || (cell.y >= GI->size) || (cell.x < 0) || (cell.x >= GI->size)){
        return false;
    }
    
    //Agentがいたら移動無効
    for(int i=0; i<4; i++){
        if(i == GI->id) { continue; }
        if(GI->positions[i] == cell){
            return false;
        }
    }

    return true;
}

//四隅も禁止
bool IntelligentAwarness::JudgeMoveCell2(const Cell& cell){
    //フィールド外は無効
    if((cell.y < 0) || (cell.y >= GI->size) || (cell.x < 0) || (cell.x >= GI->size)){
        return false;
    }

    //四隅は無効
    if( ((cell.x == 0)||(cell.x == (GI->size - 1))) && ((cell.y == 0)||(cell.y == (GI->size -1)))){
        return false;
    }
    
    //Agentがいたら移動無効
    for(int i=0; i<4; i++){
        if(i == GI->id) { continue; }
        if(GI->positions[i] == cell){
            return false;
        }
    }

    return true;
}

bool IntelligentAwarness::JudgeOutFieldCell(const Cell& cell){
    //フィールド外は無効
    if((cell.y < 0) || (cell.y >= GI->size) || (cell.x < 0) || (cell.x >= GI->size)){
        return false;
    }
    return true;
}

bool IntelligentAwarness::JudgeOutFieldCell2(const Cell& cell){
    //フィールド外は無効
    if((cell.y < 0) || (cell.y >= GI->size) || (cell.x < 0) || (cell.x >= GI->size)){
        return false;
    }

    //四隅は無効
    if( ((cell.x == 0)||(cell.x == (GI->size - 1))) && ((cell.y == 0)||(cell.y == (GI->size -1)))){
        return false;
    }

    return true;
}

//穴ならtrue, 穴じゃなければfalse
bool IntelligentAwarness::JudgeHoleCell(const Cell& cell){
    if(GI->holes_list.find(cell) == GI->holes_list.end()){
        return false;
    }
    return true;
}

//最初の進むべき方向を返す。　たどりつけない場合はSKIP
IntelligentAwarness::Command IntelligentAwarness::JudgeMoveTargetForSamurai(const Cell& target, const Cell& goal){
    //cout << "t:" << target << "g:" << goal << endl;
    map<Cell, bool> checkMap;
    for(int i=0; i < GI->size; i++){
        for(int j=0; j < GI->size; j++){
            checkMap.emplace(Cell(i, j), false);
        }
    }

    queue<SuperCell> que;
    que.push(SuperCell(target.x, target.y, Command::SKIP));
    bool initFlg = true;
    while(que.size() > 0){
        SuperCell tmp = que.front();
        que.pop();
        if(tmp == goal){    //一致していたら
            return tmp.direction;
        }
        if(checkMap[Cell(tmp.x, tmp.y)] == true){
            continue;
        }
        checkMap[Cell(tmp.x, tmp.y)] = true;

        //cout << tmp << endl;
        if(initFlg){
            initFlg = false;
            //通常移動
            if((JudgeMoveCell(Cell(tmp.x, tmp.y - 1))) && (!JudgeHoleCell(Cell(tmp.x, tmp.y-1))))
                que.push(SuperCell(tmp.x, tmp.y - 1, Command::M_U));
            if((JudgeMoveCell(Cell(tmp.x, tmp.y + 1))) && (!JudgeHoleCell(Cell(tmp.x, tmp.y+1))))
                que.push(SuperCell(tmp.x, tmp.y + 1, Command::M_D));
            if((JudgeMoveCell(Cell(tmp.x - 1, tmp.y))) && (!JudgeHoleCell(Cell(tmp.x-1, tmp.y))))
                que.push(SuperCell(tmp.x - 1, tmp.y, Command::M_L));
            if((JudgeMoveCell(Cell(tmp.x + 1, tmp.y))) && (!JudgeHoleCell(Cell(tmp.x+1, tmp.y))))
                que.push(SuperCell(tmp.x + 1, tmp.y, Command::M_R));
            //穴埋め(1ターン遅い)
            if(JudgeMoveCell(Cell(tmp.x, tmp.y - 1)))
                que.push(SuperCell(tmp.x, tmp.y - 1, Command::F_U));
            if(JudgeMoveCell(Cell(tmp.x, tmp.y + 1))) 
                que.push(SuperCell(tmp.x, tmp.y + 1, Command::F_D));
            if(JudgeMoveCell(Cell(tmp.x - 1, tmp.y))) 
                que.push(SuperCell(tmp.x - 1, tmp.y, Command::F_L));
            if(JudgeMoveCell(Cell(tmp.x + 1, tmp.y)))
                que.push(SuperCell(tmp.x + 1, tmp.y, Command::F_R));
        }else{
            if((JudgeOutFieldCell(Cell(tmp.x, tmp.y - 1))) && (!JudgeHoleCell(Cell(tmp.x, tmp.y-1))))
                que.push(SuperCell(tmp.x, tmp.y - 1, tmp.direction));
            if((JudgeOutFieldCell(Cell(tmp.x, tmp.y + 1))) && (!JudgeHoleCell(Cell(tmp.x, tmp.y+1))))
                que.push(SuperCell(tmp.x, tmp.y + 1, tmp.direction));
            if((JudgeOutFieldCell(Cell(tmp.x - 1, tmp.y))) && (!JudgeHoleCell(Cell(tmp.x-1, tmp.y))))
                que.push(SuperCell(tmp.x - 1, tmp.y, tmp.direction));
            if((JudgeOutFieldCell(Cell(tmp.x + 1, tmp.y))) && (!JudgeHoleCell(Cell(tmp.x+1, tmp.y))))
                que.push(SuperCell(tmp.x + 1, tmp.y, tmp.direction));
            //穴埋め(1ターン遅い)
            if(JudgeOutFieldCell(Cell(tmp.x, tmp.y - 1)))
                que.push(SuperCell(tmp.x, tmp.y - 1, tmp.direction));
            if(JudgeOutFieldCell(Cell(tmp.x, tmp.y + 1))) 
                que.push(SuperCell(tmp.x, tmp.y + 1, tmp.direction));
            if(JudgeOutFieldCell(Cell(tmp.x - 1, tmp.y))) 
                que.push(SuperCell(tmp.x - 1, tmp.y, tmp.direction));
            if(JudgeOutFieldCell(Cell(tmp.x + 1, tmp.y)))
                que.push(SuperCell(tmp.x + 1, tmp.y, tmp.direction));
        }
    }

    return Command::SKIP;
}

//最初の進むべき方向を返す。　たどりつけない場合はSKIP
IntelligentAwarness::Command IntelligentAwarness::JudgeMoveTargetForDog(const Cell& target, const Cell& goal){
    map<Cell, bool> checkMap;
    for(int i=0; i < GI->size; i++){
        for(int j=0; j < GI->size; j++){
            checkMap.emplace(Cell(i, j), false);
        }
    }

    queue<SuperCell> que;
    que.push(SuperCell(target.x, target.y, Command::SKIP));
    bool initFlg = true;
    while(que.size() > 0){
        SuperCell tmp = que.front();
        que.pop();
        if(tmp == goal){    //一致していたら
            return tmp.direction;
        }
        if(checkMap[Cell(tmp.x, tmp.y)] == true){
            continue;
        }
        checkMap[Cell(tmp.x, tmp.y)] = true;

        //cout << tmp << endl;
        if(initFlg){
            initFlg = false;
            if((JudgeMoveCell(Cell(tmp.x, tmp.y - 1))) && (!JudgeHoleCell(Cell(tmp.x, tmp.y-1))))
                que.push(SuperCell(tmp.x, tmp.y - 1, Command::M_U));
            if((JudgeMoveCell(Cell(tmp.x, tmp.y + 1))) && (!JudgeHoleCell(Cell(tmp.x, tmp.y+1))))
                que.push(SuperCell(tmp.x, tmp.y + 1, Command::M_D));
            if((JudgeMoveCell(Cell(tmp.x - 1, tmp.y))) && (!JudgeHoleCell(Cell(tmp.x-1, tmp.y))))
                que.push(SuperCell(tmp.x - 1, tmp.y, Command::M_L));
            if((JudgeMoveCell(Cell(tmp.x + 1, tmp.y))) && (!JudgeHoleCell(Cell(tmp.x+1, tmp.y))))
                que.push(SuperCell(tmp.x + 1, tmp.y, Command::M_R));
            
            if((JudgeMoveCell(Cell(tmp.x - 1, tmp.y - 1))) && (!JudgeHoleCell(Cell(tmp.x-1, tmp.y-1))))
                que.push(SuperCell(tmp.x - 1, tmp.y - 1, Command::M_L_U));
            if((JudgeMoveCell(Cell(tmp.x - 1, tmp.y + 1))) && (!JudgeHoleCell(Cell(tmp.x-1, tmp.y+1))))
                que.push(SuperCell(tmp.x - 1, tmp.y + 1, Command::M_L_D));
            if((JudgeMoveCell(Cell(tmp.x + 1, tmp.y - 1))) && (!JudgeHoleCell(Cell(tmp.x+1, tmp.y-1))))
                que.push(SuperCell(tmp.x + 1, tmp.y - 1, Command::M_R_U));
            if((JudgeMoveCell(Cell(tmp.x + 1, tmp.y + 1))) && (!JudgeHoleCell(Cell(tmp.x+1, tmp.y+1))))
                que.push(SuperCell(tmp.x + 1, tmp.y + 1, Command::M_R_D));
        }else{
            if((JudgeOutFieldCell(Cell(tmp.x, tmp.y - 1))) && (!JudgeHoleCell(Cell(tmp.x, tmp.y-1))))
                que.push(SuperCell(tmp.x, tmp.y - 1, tmp.direction));
            if((JudgeOutFieldCell(Cell(tmp.x, tmp.y + 1))) && (!JudgeHoleCell(Cell(tmp.x, tmp.y+1))))
                que.push(SuperCell(tmp.x, tmp.y + 1, tmp.direction));
            if((JudgeOutFieldCell(Cell(tmp.x - 1, tmp.y))) && (!JudgeHoleCell(Cell(tmp.x-1, tmp.y))))
                que.push(SuperCell(tmp.x - 1, tmp.y, tmp.direction));
            if((JudgeOutFieldCell(Cell(tmp.x + 1, tmp.y))) && (!JudgeHoleCell(Cell(tmp.x+1, tmp.y))))
                que.push(SuperCell(tmp.x + 1, tmp.y, tmp.direction));

            if((JudgeMoveCell(Cell(tmp.x - 1, tmp.y - 1))) && (!JudgeHoleCell(Cell(tmp.x-1, tmp.y-1))))
                que.push(SuperCell(tmp.x - 1, tmp.y - 1, tmp.direction));
            if((JudgeMoveCell(Cell(tmp.x - 1, tmp.y + 1))) && (!JudgeHoleCell(Cell(tmp.x-1, tmp.y+1))))
                que.push(SuperCell(tmp.x - 1, tmp.y + 1, tmp.direction));
            if((JudgeMoveCell(Cell(tmp.x + 1, tmp.y - 1))) && (!JudgeHoleCell(Cell(tmp.x+1, tmp.y-1))))
                que.push(SuperCell(tmp.x + 1, tmp.y - 1, tmp.direction));
            if((JudgeMoveCell(Cell(tmp.x + 1, tmp.y + 1))) && (!JudgeHoleCell(Cell(tmp.x+1, tmp.y+1))))
                que.push(SuperCell(tmp.x + 1, tmp.y + 1, tmp.direction));
        }
    }
    return Command::SKIP;
}

bool IntelligentAwarness::GetExietSamurai(const Cell& target, int AgentID){
    Cell tmp;
    for(int i=0; i<4; i++){
        switch(i){
            case 0:
                tmp = Cell(target.x, target.y - 1);
                break;
            case 1:
                tmp = Cell(target.x, target.y + 1);
                break;
            case 2:
                tmp = Cell(target.x + 1, target.y);
                break;
            case 3:
                tmp = Cell(target.x - 1, target.y);
                break;
            default:
                break;
        }
        if((tmp.y < 0) || (tmp.y >= GI->size) || (tmp.x < 0) || (tmp.x >= GI->size)){
            continue;
        }
        if(tmp == GI->positions[AgentID]){
            return true;
        }
    }
    return false;
}

bool IntelligentAwarness::GetExietDog(const Cell& target, int AgentID){
    Cell tmp;
    for(int i=0; i<8; i++){
        switch(i){
            case 0:
                tmp = Cell(target.x, target.y - 1);
                break;
            case 1:
                tmp = Cell(target.x, target.y + 1);
                break;
            case 2:
                tmp = Cell(target.x + 1, target.y);
                break;
            case 3:
                tmp = Cell(target.x - 1, target.y);
                break;
            case 4:
                tmp = Cell(target.x - 1, target.y - 1);
                break;
            case 5:
                tmp = Cell(target.x + 1, target.y - 1);
                break;
            case 6:
                tmp = Cell(target.x + 1, target.y + 1);
                break;
            case 7:
                tmp = Cell(target.x - 1, target.y + 1);
                break;
            default:
                break;
        }
        if((tmp.y < 0) || (tmp.y >= GI->size) || (tmp.x < 0) || (tmp.x >= GI->size)){
            continue;
        }
        if(tmp == GI->positions[AgentID]){
            return true;
        }
    }
    return false;
}

IntelligentAwarness::Command IntelligentAwarness::GetAgent2AgentDirect(const Cell& from, const Cell& to){
    if(to.y > from.y){ //Down
        if(to.x > from.x)
            return Command::M_R_D;
        else if(to.x < from.x)
            return Command::M_L_D;
        else
            return Command::M_D;
    }else if(to.y < from.y){ //Up
        if(to.x > from.x)
            return Command::M_R_U;
        else if(to.x < from.x)
            return Command::M_L_U;
        else
            return Command::M_U;
    }else{
        if(to.x > from.x)
            return Command::M_R;
        else if(to.x < from.x)
            return Command::M_L;
        else
            return Command::SKIP;
    }
}

bool IntelligentAwarness::PreActiveSuccess(){
    if(GI->plans[MY_AGENT_SAMURAI] == GI->actions[MY_AGENT_SAMURAI])
        return true;
    return false;
}