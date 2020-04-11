#include "MoveFreeInfo.h"

MoveFreeInfo::MoveFreeInfo(GameInfo *GI) : Agent(GI)
{
    updateMoveRate();
}

MoveFreeInfo::~MoveFreeInfo()
{
}

void MoveFreeInfo::Update(GameInfo *GI)
{
    this->GI = GI;
    updateMoveRate();
}

float MoveFreeInfo::GetMoveRate(int id)
{
    return this->moveRate[id];
}

void MoveFreeInfo::updateMoveRate()
{
    Cell tmp;
    for (int i = 0; i < 4; i++)
    {
        float addRate = (i <= 1) ? 0.25F : 0.125F;
        this->moveRate[i] = 0.0F;
        //4近傍
        for (int j = 0; j < 4; j++)
        {
            switch (j)
            {
            case 0:
                tmp = Cell(GI->positions[i].x + 1, GI->positions[i].y); //R
                break;
            case 1:
                tmp = Cell(GI->positions[i].x - 1, GI->positions[i].y); //L
                break;
            case 2:
                tmp = Cell(GI->positions[i].x, GI->positions[i].y - 1); //U
                break;
            case 3:
                tmp = Cell(GI->positions[i].x, GI->positions[i].y + 1); //D
                break;
            default:
                break;
            }

            if (JudgeMoveCell(tmp))
            {
                if (JudgeHoleCell(tmp) == false) //穴じゃない？
                {
                    this->moveRate[i] += addRate;//動ける
                }
            }
        }
        if (i >= 2)
        { //犬
            //8近傍
            for (int j = 0; j < 4; j++)
            {
                switch (j)
                {
                case 0:
                    tmp = Cell(GI->positions[i].x + 1, GI->positions[i].y - 1); //RU
                    break;
                case 1:
                    tmp = Cell(GI->positions[i].x - 1, GI->positions[i].y - 1); //LU
                    break;
                case 2:
                    tmp = Cell(GI->positions[i].x + 1, GI->positions[i].y + 1); //RD
                    break;
                case 3:
                    tmp = Cell(GI->positions[i].x - 1, GI->positions[i].y + 1); //LD
                    break;
                default:
                    break;
                }

                if (JudgeMoveCell(tmp))
                {
                    if (JudgeHoleCell(tmp) == false) //穴じゃない？
                    {
                        this->moveRate[i] += addRate;//動ける
                    }
                }
            }
        }
    }
}

void MoveFreeInfo::DebugPrintMap()
{
    cout << "debug_move_free_info " << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << "Agent" << i << " : " << moveRate[i] << endl;
    }
    cout << "debug_move_free_info end" << endl;
}