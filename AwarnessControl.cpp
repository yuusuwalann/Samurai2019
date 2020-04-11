#include "AwarnessControl.h"

AwarnessControl::AwarnessControl(GameInfo* GI):GI(GI){
    targetAwarness = nullptr;
}

void AwarnessControl::Update(GameInfo* gi){
    this->GI = gi;
}

void AwarnessControl::PushAwarness(Awarness* aw){
    this->awarnessList.push_back(aw);
}

int AwarnessControl::DicideAwarness(){
    //debug
    /*
    cout << "awrnessList" << endl;
    for(auto awn :awarnessList){
        cout << *awn;
    }
    */

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
            targetAwarness->AddAttentionRate(targetAwarness->GetAttentionRate() + 20.0F);
        }else{
            if(targetAwarness->GetKind() != Awarness::CHAIS){
                targetAwarness->error_cnt += 1;
                targetAwarness->AddAttentionRate( targetAwarness->GetAttentionRate() - (5.0F * targetAwarness->error_cnt));
                //エラー地点を目標とする意識をすべて低下させる。
            }
        }
    }
    
    //意識から消えた？　てたら削除
    for(auto itr = awarnessList.begin(); itr != awarnessList.end();){
        if((*itr)->GetAttentionRate() <= -150.0F){
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
    bool digAwn = false;
    
    while(ngList.size() != awarnessList.size()){
        float maxRate = -9999.0F;
        //最も関心が高い意識を選択(Dig優先)
        for(auto awn : awarnessList){
            //NGチェック
            bool ngCheck = true;
            for(auto awn2 : ngList) { if(awn2 == awn){ ngCheck = false; break;}}
            if(ngCheck == false){continue;}

            if( (maxRate < awn->GetAttentionRate()) 
                || ( (maxRate == awn->GetAttentionRate())&&(digAwn == false))){
                maxRate = awn->GetAttentionRate();
                target = awn;
                digAwn = awn->GetKind() == awn->DIG ? true : false;
            }
        }
        int cmd = (int)(target->GetIntellAwa()->JudgeCommand(target));
        //その行動は可能か
        //セル動作チェック
        Cell myCell = GI->positions[IntelligentAwarness::MY_AGENT_SAMURAI];
        bool tmpFlg = false;
        switch(cmd){
            case (int)IntelligentAwarness::SKIP:
                ngList.push_back(target);
                tmpFlg = true;
                break;
            case (int)IntelligentAwarness::M_D:
            case (int)IntelligentAwarness::D_D:
            case (int)IntelligentAwarness::F_D:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x, myCell.y + 1)) == false){
                    ngList.push_back(target);
                    tmpFlg = true;
                }
                break;
           case (int)IntelligentAwarness::M_U:
           case (int)IntelligentAwarness::D_U:
           case (int)IntelligentAwarness::F_U:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x, myCell.y - 1)) == false){
                    ngList.push_back(target);
             
                    tmpFlg = true;
                }
                break;
            case (int)IntelligentAwarness::M_L:
            case (int)IntelligentAwarness::D_L:
            case (int)IntelligentAwarness::F_L:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x - 1, myCell.y)) == false){
                    ngList.push_back(target);
                    tmpFlg = true;
                }
                break;
            case (int)IntelligentAwarness::M_R:
            case (int)IntelligentAwarness::D_R:
            case (int)IntelligentAwarness::F_R:
                if(target->GetIntellAwa()->JudgeMoveCell(Cell(myCell.x + 1, myCell.y)) == false){
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

        //選ばれた意識により行動を選択する
        targetAwarness = target;

        //cout << "Decide=> cmd:" << targetAwarness->GetKind() << "Target" << targetAwarness->GetGoalValue();
        
        return cmd;
    }

    targetAwarness = nullptr;
    return  (int)IntelligentAwarness::SKIP;
}

bool AwarnessControl::ExistAwarnessList(Awarness* aws){
    for(auto thisAws : awarnessList){
        if(*thisAws == *aws){
            return true;
        }
    }
    return false;
}
