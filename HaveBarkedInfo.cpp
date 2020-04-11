#include "HaveBarkedInfo.h"

HaveBarkedInfo::HaveBarkedInfo(GameInfo* GI):GI(GI){
    //初期化
    for(int i=0; i < GI->size; i++){
        for(int j=0; j < GI->size; j++){
            haveBarkedMap.emplace(Cell(i, j), false);
        }
    }
    //初期に見えている財宝リスト
    if(GI->revealed_num != 0){
        for(auto itr : GI->revealed_list){
            haveBarkedMap[itr.first] = true;
        }
    }
}

HaveBarkedInfo::~HaveBarkedInfo(){

}

void HaveBarkedInfo::Update(GameInfo* GI){
    this->GI = GI;
    
    haveBarkedMap[GI->positions[Agent::MY_AGENT_DOG]] = true;
    haveBarkedMap[GI->positions[Agent::ENE_AGENT_DOG]] = true;
    //自分の犬の8近傍は犬は把握している
    Cell tmp = GI->positions[Agent::MY_AGENT_DOG];
    if(judgeOutFieldCell(Cell(tmp.x, tmp.y - 1)))
        haveBarkedMap[Cell(tmp.x, tmp.y - 1)] = true;
    if(judgeOutFieldCell(Cell(tmp.x, tmp.y + 1)))
        haveBarkedMap[Cell(tmp.x, tmp.y + 1)] = true;
    if(judgeOutFieldCell(Cell(tmp.x + 1, tmp.y)))
        haveBarkedMap[Cell(tmp.x + 1, tmp.y)] = true;
    if(judgeOutFieldCell(Cell(tmp.x - 1, tmp.y)))
        haveBarkedMap[Cell(tmp.x - 1, tmp.y)] = true;

    if(judgeOutFieldCell(Cell(tmp.x - 1, tmp.y - 1)))
        haveBarkedMap[Cell(tmp.x - 1, tmp.y - 1)] = true;
    if(judgeOutFieldCell(Cell(tmp.x + 1, tmp.y - 1)))
        haveBarkedMap[Cell(tmp.x + 1, tmp.y - 1)] = true;
    if(judgeOutFieldCell(Cell(tmp.x - 1, tmp.y + 1)))
        haveBarkedMap[Cell(tmp.x - 1, tmp.y + 1)] = true;
    if(judgeOutFieldCell(Cell(tmp.x + 1, tmp.y + 1)))
        haveBarkedMap[Cell(tmp.x + 1, tmp.y + 1)] = true;

    //すでに掘ってあるセルも吠え済みとする
    if(GI->hole_num != 0){
    	for (auto itr : GI->holes_list) {
            haveBarkedMap[itr] = true;
	    }
    }
}

bool HaveBarkedInfo::GetBarkedInfo(const Cell& cell){
	return haveBarkedMap[cell];
}

void HaveBarkedInfo::DebugPrintMap(){
    cout << "debug_have_barked_info " << endl;
    for(int y=0; y < GI->size; y++){
        for(int x=0; x < GI->size; x++){
            cout << haveBarkedMap[Cell(x, y)] << " ";
        }
        cout << endl;
    }
    cout << "debug_have_barked_info end" << endl;
}

bool HaveBarkedInfo::judgeOutFieldCell(const Cell& cell){
    //フィールド外は無効
    if((cell.y < 0) || (cell.y >= GI->size) || (cell.x < 0) || (cell.x >= GI->size)){
        return false;
    }
    return true;
}