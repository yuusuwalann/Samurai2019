#pragma once
#include <iostream>

using namespace std;

struct Cell {
	int x, y;
	Cell() {}
	Cell(unsigned char x, unsigned char y);
	Cell(istream& in);
	bool operator==(const Cell &another) const;
	bool operator<(const Cell &another) const;
};

ostream & operator << (ostream &out, const Cell &cell);