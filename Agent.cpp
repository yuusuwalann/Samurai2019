#include "Agent.h"

using namespace std;

class SuperCell : public Cell{
public:
    Agent::Command direction;
    SuperCell(int x, int y, Agent::Command direction)
        :Cell(x, y), direction(direction){;}
    ~SuperCell(){;}
};

Agent::Agent(const GameInfo *GI):GI(GI)
{
    MY_AGENT_SAMURAI = (GI->id % 2) == 0 ? 0 : 1;
    ENE_AGENT_SAMURAI = (GI->id %2 ) == 0 ? 1 : 0;
    MY_AGENT_DOG = (GI->id %2) == 0 ? 2 : 3;
    ENE_AGENT_DOG = (GI->id %2) == 0 ? 3 : 2;
    //total埋蔵金算出
	int TOTAL_TORESURE = GI->scores[0] + GI->scores[1] + GI->remaining;
	for(auto tra : GI->revealed_list) { TOTAL_TORESURE += tra.second; }
}


Agent::~Agent()
{
}

bool Agent::JudgeMoveCell(const Cell& cell){
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

bool Agent::JudgeOutFieldCell(const Cell& cell){
    //フィールド外は無効
    if((cell.y < 0) || (cell.y >= GI->size) || (cell.x < 0) || (cell.x >= GI->size)){
        return false;
    }
    return true;
}

//穴ならtrue, 穴じゃなければfalse
bool Agent::JudgeHoleCell(const Cell& cell){
    if(GI->holes_list.find(cell) == GI->holes_list.end()){
        return false;
    }
    return true;
}

//最初の進むべき方向を返す。　たどりつけない場合はSKIP
Agent::Command Agent::JudgeMoveTargetForSamurai(const Cell& target, const Cell& goal){
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
            if(JudgeMoveCell(Cell(tmp.x, tmp.y - 1)))
                que.push(SuperCell(tmp.x, tmp.y - 1, tmp.direction));
            if(JudgeMoveCell(Cell(tmp.x, tmp.y + 1))) 
                que.push(SuperCell(tmp.x, tmp.y + 1, tmp.direction));
            if(JudgeMoveCell(Cell(tmp.x - 1, tmp.y))) 
                que.push(SuperCell(tmp.x - 1, tmp.y, tmp.direction));
            if(JudgeMoveCell(Cell(tmp.x + 1, tmp.y)))
                que.push(SuperCell(tmp.x + 1, tmp.y, tmp.direction));
        }
    }

    return Command::SKIP;
}

//最初の進むべき方向を返す。　たどりつけない場合はSKIP
Agent::Command Agent::JudgeMoveTargetForDog(const Cell& target, const Cell& goal){
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

bool Agent::GetExietSamurai(const Cell& target, int agentID){
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
        if(tmp == GI->positions[agentID]){
            return true;
        }
    }
    return false;
}

bool Agent::GetExietDog(const Cell& target, int agentID){
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
        if(tmp == GI->positions[agentID]){
            return true;
        }
    }
    return false;
}

Agent::Command Agent::GetAgent2AgentDirect(const Cell& from, const Cell& to){
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

void Agent::PrintCommand(Command cmd){
    switch(cmd){
        case Command::M_U:
            cout << "Move_Up";
            break;
        case Command::M_L_U:
            cout << "Move_LeftUp";
            break;
        case Command::M_L:
            cout << "Move_Left";
            break;
        case Command::M_L_D:
            cout << "Move_LeftDown";
            break;
        case Command::M_D:
            cout << "Move_Down";
            break;
        case Command::M_R_D:
            cout << "Move_RightDown";
            break;
        case Command::M_R:
            cout << "Move_Right";
            break;
        case Command::M_R_U:
            cout << "Move_RightUp";
            break;
        case Command::D_D:
            cout << "Dig_Down";
            break;
        case Command::D_U:
            cout << "Dig_Up";
            break;
        case Command::D_L:
            cout << "Dig_Left";
            break;
        case Command::D_R:
            cout << "Dig_Right";
            break;
        case Command::F_D:
            cout << "Fil_Down";
            break;
        case Command::F_U:
            cout << "Fil_Up";
            break;
        case Command::F_L:
            cout << "Fil_Left";
            break;
        case Command::F_R:
            cout << "Fil_Right";
            break;
        case Command::SKIP:
            cout << "Skip";
            break;
        default:
            cout << "NG_COMMAND";
            break;
    }
}

ostream & operator << (ostream &out, const Agent::Command &cmd) {
        switch(cmd){
        case Agent::Command::M_U:
            out << "Move_Up";
            break;
        case Agent::Command::M_L_U:
            out << "Move_LeftUp";
            break;
        case Agent::Command::M_L:
            out << "Move_Left";
            break;
        case Agent::Command::M_L_D:
            out << "Move_LeftDown";
            break;
        case Agent::Command::M_D:
            out << "Move_Down";
            break;
        case Agent::Command::M_R_D:
            out << "Move_RightDown";
            break;
        case Agent::Command::M_R:
            out << "Move_Right";
            break;
        case Agent::Command::M_R_U:
            out << "Move_RightUp";
            break;
        case Agent::Command::D_D:
            out << "Dig_Down";
            break;
        case Agent::Command::D_U:
            out << "Dig_Up";
            break;
        case Agent::Command::D_L:
            out << "Dig_Left";
            break;
        case Agent::Command::D_R:
            out << "Dig_Right";
            break;
        case Agent::Command::F_D:
            out << "Fil_Down";
            break;
        case Agent::Command::F_U:
            out << "Fil_Up";
            break;
        case Agent::Command::F_L:
            out << "Fil_Left";
            break;
        case Agent::Command::F_R:
            out << "Fil_Right";
            break;
        case Agent::Command::SKIP:
            out << "Skip";
            break;
        default:
            out << "NG_Command";
            break;
    }

	return out;
}