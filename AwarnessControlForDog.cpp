#include "AwarnessControlForDog.h"

AwarnessControlForDog::AwarnessControlForDog(GameInfo* GI):GI(GI){
    targetAwarness = nullptr;
}

void AwarnessControlForDog::Update(GameInfo* gi){
    this->GI = gi;
}

void AwarnessControlForDog::PushAwarness(Awarness* aw){
    this->awarnessList.push_back(aw);
}

int AwarnessControlForDog::DicideAwarness(int samuraiCmd){
    //debug
    /*
    cout << "awrnessList" << endl;
    for(auto awn :awarnessList){
        cout << *awn;
    }
    */

    //samuraiのターゲット位置
    Cell mySamuraiTarget = GI->positions[IntelligentAwarness::MY_AGENT_SAMURAI];
    if( (samuraiCmd == IntelligentAwarness::M_D)
        ||(samuraiCmd == IntelligentAwarness::D_D)
        ||(samuraiCmd == IntelligentAwarness::F_D) ){
            mySamuraiTarget = Cell(mySamuraiTarget.x, mySamuraiTarget.y + 1); 
    }else if( (samuraiCmd == IntelligentAwarness::M_U)
        ||(samuraiCmd == IntelligentAwarness::D_U)
        ||(samuraiCmd == IntelligentAwarness::F_U) ){
            mySamuraiTarget = Cell(mySamuraiTarget.x, mySamuraiTarget.y - 1); 
    }else if( (samuraiCmd == IntelligentAwarness::M_R)
        ||(samuraiCmd == IntelligentAwarness::D_R)
        ||(samuraiCmd == IntelligentAwarness::F_R) ){
            mySamuraiTarget = Cell(mySamuraiTarget.x + 1, mySamuraiTarget.y); 
    }else if( (samuraiCmd == IntelligentAwarness::M_L)
        ||(samuraiCmd == IntelligentAwarness::D_L)
        ||(samuraiCmd == IntelligentAwarness::F_L) ){
            mySamuraiTarget = Cell(mySamuraiTarget.x - 1, mySamuraiTarget.y); 
    }else{
        ;
    }

    //cout << "SamuraiCommand" << samuraiCmd << " SamuraiCell:" << mySamuraiTarget;

    //update処理
    for(auto awa : awarnessList){
        awa->Update();  //意識の低下
    }

    //意識は完了した? してたら削除
    for(auto itr = awarnessList.begin(); itr != awarnessList.end();){
        IntelligentAwarness* intellAddr = (*itr)->GetIntellAwa();
        if(intellAddr->CheckComplate(*itr)){
            awarnessList.erase(itr);
            continue;
        }
        itr++;
    }

    //意識の再検討
    for(auto awa : awarnessList){
        IntelligentAwarness* intellAddr = (awa)->GetIntellAwa();
        intellAddr->UpdateAwarness(awa);  //意識の低下
    }


    //前回行動中の意識は集中（ただし、行動が失敗していないときに限り)
    if(targetAwarness != nullptr){
        if(targetAwarness->GetIntellAwa()->PreActiveSuccess()){
            targetAwarness->AddAttentionRate(targetAwarness->GetInitRate());
        }else{
            targetAwarness->error_cnt += 1;
            targetAwarness->AddAttentionRate( targetAwarness->GetAttentionRate() - (3.0F * targetAwarness->error_cnt) );
            //エラー地点を目標とする意識をすべて低下させる。
        }
    }
    
    //意識から消えた？　てたら削除
    for(auto itr = awarnessList.begin(); itr != awarnessList.end();){
        if((*itr)->GetAttentionRate() <= -80.0F){
            awarnessList.erase(itr);
            continue;
        }
        itr++;
    }

    //debug
    /*
    cout << "awrnessList(endProc)" << endl;
    for(auto awn :awarnessList){
        cout << *awn;
    }
    */


    vector<Awarness*> ngList;
    Awarness* target = nullptr;
    
    while(ngList.size() != awarnessList.size()){
        float maxRate = -9999.0F;
        //最も関心が高い意識を選択
        for(auto awn : awarnessList){
            //NGチェック
            bool ngCheck = true;
            for(auto awn2 : ngList) { if(awn2 == awn){ ngCheck = false; break;}}
            if(ngCheck == false){continue;}

            if( maxRate < awn->GetAttentionRate()){
                maxRate = awn->GetAttentionRate();
                target = awn;
            }
        }
        int cmd = (int)(target->GetIntellAwa()->JudgeCommand(target));

        //その行動は可能か
        //セル動作チェック
        Cell myCell = GI->positions[IntelligentAwarness::MY_AGENT_DOG];
        bool tmpFlg = false;
        switch(cmd){
            case (int)IntelligentAwarness::SKIP:
                ngList.push_back(target);
                tmpFlg = true;
                break;
            case (int)IntelligentAwarness::M_D:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x, myCell.y + 1)) == false){
                    ngList.push_back(target);
                    tmpFlg = true;
                }
                break;
            case (int)IntelligentAwarness::M_L_D:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x - 1, myCell.y + 1)) == false){
                    ngList.push_back(target);
                    tmpFlg = true;
                }
                break;
            case (int)IntelligentAwarness::M_R_D:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x + 1, myCell.y + 1)) == false){
                    ngList.push_back(target);
                    tmpFlg = true;
                }
                break;
           case (int)IntelligentAwarness::M_U:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x, myCell.y - 1)) == false){
                    ngList.push_back(target);
                    tmpFlg = true;
                }
                break;
           case (int)IntelligentAwarness::M_L_U:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x - 1, myCell.y - 1)) == false){
                    ngList.push_back(target);
                    tmpFlg = true;
                }
                break;
           case (int)IntelligentAwarness::M_R_U:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x + 1, myCell.y - 1)) == false){
                    ngList.push_back(target);
                    tmpFlg = true;
                }
                break;
            default:
                break;
        }
        if(tmpFlg){
             continue; 
        }

        //侍の行動とかぶっていないか
        Cell myDogPos = GI->positions[IntelligentAwarness::MY_AGENT_DOG];
        bool samuSameFlg = true;
        switch((int)cmd){
            case (int)IntelligentAwarness::M_D:
                if(mySamuraiTarget == Cell(myDogPos.x, myDogPos.y + 1)){
                    samuSameFlg = false;
                }
                break;
            case (int)IntelligentAwarness::M_U:
                if(mySamuraiTarget == Cell(myDogPos.x, myDogPos.y - 1)){
                    samuSameFlg = false;
                }
                break;
            case (int)IntelligentAwarness::M_L:
                if(mySamuraiTarget == Cell(myDogPos.x - 1, myDogPos.y)){
                    samuSameFlg = false;
                }
                break;
            case (int)IntelligentAwarness::M_R:
                if(mySamuraiTarget == Cell(myDogPos.x + 1, myDogPos.y)){
                    samuSameFlg = false;
                }
                break;
            case (int)IntelligentAwarness::M_R_D:
                if(mySamuraiTarget == Cell(myDogPos.x + 1, myDogPos.y + 1)){
                    samuSameFlg = false;
                }
                break;
            case (int)IntelligentAwarness::M_R_U:
                if(mySamuraiTarget == Cell(myDogPos.x + 1, myDogPos.y - 1)){
                    samuSameFlg = false;
                }
                break;
            case (int)IntelligentAwarness::M_L_D:
                if(mySamuraiTarget == Cell(myDogPos.x - 1, myDogPos.y + 1)){
                    samuSameFlg = false;
                }
                break;
            case (int)IntelligentAwarness::M_L_U:
                if(mySamuraiTarget == Cell(myDogPos.x - 1, myDogPos.y - 1)){
                    samuSameFlg = false;
                }
                break;
            default:
                break;
        }
        if(samuSameFlg == false){
            ngList.push_back(target);
            continue;
        }
        
        //選ばれた意識により行動を選択する
        targetAwarness = target;
        return cmd;

    }

    targetAwarness = nullptr;
    return  (int)IntelligentAwarness::SKIP;
}

bool AwarnessControlForDog::ExistAwarnessList(Awarness* aws){
    for(auto thisAws : awarnessList){
        if(*thisAws == *aws){
            return true;
        }
    }
    return false;
}
