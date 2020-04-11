#include "RevealedMapInfo.h"

RevealedMapInfo::RevealedMapInfo(GameInfo* GI):GI(GI){
    //初期化
    for(int i=0; i < GI->size; i++){
        for(int j=0; j < GI->size; j++){
            predicReveMap.emplace(Cell(i, j), 0.0F);
            heatRevMap.emplace(Cell(i,j), 0.0F);
        }
    }
    //すでに埋蔵金が存在している
    if(GI->revealed_num != 0){
    	for (auto itr : GI->revealed_list) {
            predicReveMap[itr.first] = 1.0F;
            heatRevMap[itr.first] = 1.0F;
            updateHeatMap(itr.first);
	    }
    }
}

RevealedMapInfo::~RevealedMapInfo(){

}

void RevealedMapInfo::Update(GameInfo* GI){
    this->GI = GI;
    if(GI->revealed_num != 0){
    	for (auto itr : GI->revealed_list) {
            predicReveMap[itr.first] = 1.0F;
            heatRevMap[itr.first] = 1.0F;
            updateHeatMap(itr.first);
	    }
    }
}

float RevealedMapInfo::GetPredicReveInfo(const Cell& cell){
	return predicReveMap[cell];
}

void RevealedMapInfo::updateHeatMap(const Cell& cell){
    if(cell.x > 0)
        this->heatRevMap[Cell(cell.x - 1, cell.y)] += 0.5F;
    if(cell.x < (GI->size - 1))
        this->heatRevMap[Cell(cell.x + 1, cell.y)] += 0.5F;
    if(cell.y > 0)
        this->heatRevMap[Cell(cell.x, cell.y - 1)] += 0.5F;
    if(cell.y < (GI->size - 1))
        this->heatRevMap[Cell(cell.x, cell.y + 1)] += 0.5F;

    if((cell.x > 0) && (cell.y > 0))
        this->heatRevMap[Cell(cell.x - 1, cell.y - 1)] += 0.2F;
    if((cell.x < (GI->size - 1)) && (cell.y > 0))
        this->heatRevMap[Cell(cell.x + 1, cell.y - 1)] += 0.2F;
    if((cell.x > 0) && (cell.y < (GI->size - 1)))
        this->heatRevMap[Cell(cell.x - 1, cell.y + 1)] += 0.2F;
    if((cell.x < (GI->size - 1)) && (cell.y < (GI->size - 1)))
        this->heatRevMap[Cell(cell.x + 1, cell.y + 1)] += 0.2F;
}

void RevealedMapInfo::DebugPrintMap(){
    cout << "debug_revealed_map_info" << endl;
    cout << "predicReveMap =>" << endl;
    for(int y=0; y < GI->size; y++){
        for(int x=0; x < GI->size; x++){
            cout << predicReveMap[Cell(x, y)] << " ";
        }
        cout << endl;
    }
    cout << "haet_map =>" << endl;
    for(int y=0; y < GI->size; y++){
        for(int x=0; x < GI->size; x++){
            cout << heatRevMap[Cell(x, y)] << " ";
        }
        cout << endl;
    }
    cout << "debug_revealed_map_info end" << endl;
}