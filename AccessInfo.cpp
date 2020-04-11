#include "AccessInfo.h"

using namespace std;

AccessInfo::AccessInfo(GameInfo *GI) : Agent(GI)
{
    for (int i = 0; i < GI->size; i++)
    {
        for (int j = 0; j < GI->size; j++)
        {
            InfoStruct ai = {INIT_INFO_VALUE, INIT_INFO_VALUE, INIT_INFO_VALUE, INIT_INFO_VALUE};
            accessInfoMap.emplace(Cell(i, j), ai);
        }
    }
}

AccessInfo::~AccessInfo()
{
}

struct SuperCell : public Cell{
    public:
        int depth;
        SuperCell(Cell cell, int depth):Cell(cell),depth(depth){;}
        ~SuperCell(){}
};

void AccessInfo::Update(GameInfo *GI)
{
    this->GI = GI;
    for (int i = 0; i < GI->size; i++)
    {
        for (int j = 0; j < GI->size; j++)
        {
            InfoStruct ai = {INIT_INFO_VALUE, INIT_INFO_VALUE, INIT_INFO_VALUE, INIT_INFO_VALUE};
            accessInfoMap[Cell(i, j)] = ai;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        queue<SuperCell> que;
        que.push(SuperCell(GI->positions[i], 0));
        accessInfoMap[GI->positions[i]].agentDist[i] = 0;
        if (i < 2)
        { //侍
            while (que.size() > 0)
            {
                SuperCell popCell = que.front();
                que.pop();
                for (int j = 0; j < 4; j++)
                {
                    Cell tmpCell;
                    switch (j)
                    {
                    case 0: //Up
                        tmpCell = Cell(popCell.x, popCell.y - 1);
                        break;
                    case 1: //Down
                        tmpCell = Cell(popCell.x, popCell.y + 1);
                        break;
                    case 2: //Left
                        tmpCell = Cell(popCell.x - 1, popCell.y);
                        break;
                    case 3: //Right
                        tmpCell = Cell(popCell.x + 1, popCell.y);
                        break;
                    default:
                        break;
                    }

                    if (JudgeOutFieldCell(tmpCell))
                    { //枠内
                        if (accessInfoMap[tmpCell].agentDist[i] == INIT_INFO_VALUE)
                        { //未設定
                            que.push(SuperCell(tmpCell, popCell.depth + 1));
                            accessInfoMap[tmpCell].agentDist[i] = popCell.depth + 1;
                            if (JudgeHoleCell(tmpCell))                   //穴の場合
                                accessInfoMap[tmpCell].agentDist[i] += 1; //コスト2倍
                        }
                    }
                }
            }
        }
        else
        { //犬
            while (que.size() > 0)
            {
                SuperCell popCell = que.front();
                que.pop();
                for (int j = 0; j < 8; j++)
                {
                    Cell tmpCell;
                    switch (j)
                    {
                    case (0): //Up
                        tmpCell = Cell(popCell.x, popCell.y - 1);
                        break;
                    case (1): //Down
                        tmpCell = Cell(popCell.x, popCell.y + 1);
                        break;
                    case (2): //Left
                        tmpCell = Cell(popCell.x - 1, popCell.y);
                        break;
                    case (3): //Right
                        tmpCell = Cell(popCell.x + 1, popCell.y);
                        break;
                    case (4): //Up_Left
                        tmpCell = Cell(popCell.x - 1, popCell.y - 1);
                        break;
                    case (5): //Down_Left
                        tmpCell = Cell(popCell.x - 1, popCell.y + 1);
                        break;
                    case (6): //Up_Right
                        tmpCell = Cell(popCell.x + 1, popCell.y - 1);
                        break;
                    case (7): //Down_Right
                        tmpCell = Cell(popCell.x + 1, popCell.y - 1);
                        break;
                    default:
                        break;
                    }

                    if ((JudgeOutFieldCell(tmpCell))&&(JudgeHoleCell(tmpCell) == false))
                    { //枠内&穴でない(犬は穴だめ)
                        if (accessInfoMap[tmpCell].agentDist[i] == INIT_INFO_VALUE)
                        { //未設定
                            que.push(SuperCell(tmpCell, popCell.depth + 1));
                            accessInfoMap[tmpCell].agentDist[i] = popCell.depth + 1;
                        }
                    }
                }
            }
        }
    }
}

map<Cell, InfoStruct>* AccessInfo::GetAccessMapAddr()
{
    return &accessInfoMap;
}

int AccessInfo::GetAccessDistance(const Cell& cell, int id){
    return accessInfoMap[cell].agentDist[id];
}

void AccessInfo::DebugPrintMap(int id){
    cout << "debug_access_info id:" << id << endl;
    for(int y=0; y < GI->size; y++){
        for(int x=0; x < GI->size; x++){
            cout << accessInfoMap[Cell(x, y)].agentDist[id] << " ";
        }
        cout << endl;
    }
    cout << "debug_access_info end" << endl;
}