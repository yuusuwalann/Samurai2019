#include "GameInfo.h"

GameInfo::GameInfo(){
	id = 0; size = 0; step = 0;
	max_step = 0; hole_num = 0;
	revealed_num = 0; sensed_num = 0;
	remaining = 0; thinkTime = 0;
}

GameInfo::GameInfo(istream &in) {
	in >> id >> size >> step >> max_step;

	in >> hole_num;
	for (int i = 0; i < hole_num; i++) {
		holes_list.emplace(cin);
	}

	in >> revealed_num;
	for (int i = 0; i < revealed_num; i++) {
		Cell tmp(in);
		int amount; in >> amount;
		revealed_list.emplace(tmp, amount);
	}

	in >> sensed_num;
	for (int i = 0; i < sensed_num; i++) {
		Cell tmp(cin);
		int amount; in >> amount;
		sensed_list.emplace(tmp, amount);
	}

	for (auto &pos : positions) pos = Cell(in);

	for (auto &plan : plans) in >> plan;

	for (auto &action : actions) in >> action;

	for (auto &score : scores) in >> score;

	in >> remaining;

	in >> thinkTime;
}

ostream &operator << (ostream &out, const GameInfo &gameinfo) {
	out << "id: " << gameinfo.id << "\n";
	out << "size: " << gameinfo.size << "\n";
	out << "step: " << gameinfo.step << "\n";
	out << "maxStep: " << gameinfo.max_step << "\n";
	out << "Holes: ";
	for (auto cell : gameinfo.holes_list) out << cell;
	out << "Revealed: ";
	for (auto reve : gameinfo.revealed_list) out << reve.second << "@" << reve.first;
	out << "Sensed: ";
	for (auto sens : gameinfo.sensed_list) out << sens.second << "@" << sens.first;
	out << "Position: ";
	for (auto pos : gameinfo.positions) out << pos ;
	out << "Plans: ";
	for (auto plan : gameinfo.plans) out << plan << " ";
	out << endl;
	out << "Actions: ";
	for (auto act : gameinfo.actions) out << act << " ";
	out << endl;
	out << "Socre: ";
	for (auto sco : gameinfo.scores) out << sco << " ";
	out << endl;
	out << "Remaining: " << gameinfo.remaining << "\n";
	out << "ThinkTime: " << gameinfo.thinkTime << "\n";
	return out;
}