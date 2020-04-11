#include "HaveDugInfo.h"

HaveDugInfo::HaveDugInfo(GameInfo* GI):GI(GI){
    //初期化
    for(int i=0; i < GI->size; i++){
        for(int j=0; j < GI->size; j++){
            haveDugMap.emplace(Cell(i, j), false);
        }
    }
    //初期盤面の解放情報更新
    if(GI->hole_num != 0){
    	for (auto itr : GI->holes_list) {
            haveDugMap[itr] = true;
	    }
    }
}

HaveDugInfo::~HaveDugInfo(){

}

void HaveDugInfo::Update(GameInfo* GI){
    this->GI = GI;
    if(GI->hole_num != 0){
    	for (auto itr : GI->holes_list) {
            haveDugMap[itr] = true;
	    }
    }
}

//掘ったことがあればtrue
bool HaveDugInfo::GetDugInfo(const Cell& cell){
	return haveDugMap[cell];
}

void HaveDugInfo::DebugPrintMap(){
    cout << "debug_have_dug_info " << endl;
    for(int y=0; y < GI->size; y++){
        for(int x=0; x < GI->size; x++){
            cout << haveDugMap[Cell(x, y)] << " ";
        }
        cout << endl;
    }
    cout << "debug_have_dug_info end" << endl;
}