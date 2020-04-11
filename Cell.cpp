#include "Cell.h"

Cell::Cell(unsigned char x, unsigned char y) :x(x), y(y) {
	;
}

Cell::Cell(istream &in) {
	in >> x >> y;
}

bool Cell::operator<(const Cell & another) const {
	return x < another.x || (x == another.x && y < another.y);
}

bool Cell::operator==(const Cell & another) const {
	return (x == another.x) && (y == another.y);
}

ostream & operator << (ostream &out, const Cell &cell) {
	out << "x:" << cell.x << " y:" << cell.y << "\n";
	return out;
}
