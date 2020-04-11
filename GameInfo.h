#pragma once
#include <iostream>
#include <set>
#include <map>
#include "Cell.h"

using namespace std;

struct GameInfo {
	int id;				// エージェントの番号
	int size;			// 競技場のサイズ
	int step;			// ステップ番号
	int max_step;		// 最大ステップ番号
	int hole_num;		// 穴のあるセルの数
	set <Cell> holes_list;	// 穴のあるセルの位置リスト
	int revealed_num;	//公知の埋蔵金の数
	map<Cell, int> revealed_list;	// 公知の埋蔵金
	int sensed_num;		// 感知した埋蔵金の数
	map<Cell, int> sensed_list;	// 感知した埋蔵金
	Cell positions[4];	// エージェントの位置
	int plans[4];		// 直前ステップでのプレイプラン
	int actions[4];		// 直前ステップでの行動
	int scores[2];		// スコア
	int remaining;		// 残る埋蔵金
	int thinkTime;		// 残る考慮時間

	GameInfo();
	GameInfo(istream &in);
};

ostream &operator << (ostream &opt, const GameInfo &gameinfo);