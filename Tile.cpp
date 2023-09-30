#include "Tile.h"

// For aa sette labelfarge i henhold til hvor mange miner som er rundt
const map<int, Color> minesToColor{{1, Color::blue},
								   {2, Color::red},
								   {3, Color::dark_green},
								   {4, Color::dark_magenta},
								   {5, Color::dark_blue},
								   {6, Color::dark_cyan},
								   {7, Color::dark_red},
								   {8, Color::dark_yellow}};

// For aa sette Tilelabel i henhold til state
const map<Cell, string> cellToSymbol{{Cell::closed, ""},
									 {Cell::open, ""},
									 {Cell::flagged, "@<"}};

void Tile::open()
{
	if (state == Cell::closed && state != Cell::flagged) {
		state = Cell::open;
		this->set();// Setter en button som trykket paa, tilsvarer aapnet rute
		if (isMine) {
			set_label("X");
			set_label_color(minesToColor.at(2));
		}
	}
}

void Tile::flag()
{
	if (state == Cell::flagged) {
		state = Cell::closed;
		set_label(cellToSymbol.at(state));
		set_label_color(Color::gray);
	} else if (state == Cell::closed) {
		state = Cell::flagged;
		set_label(cellToSymbol.at(state));
		set_label_color(minesToColor.at(1));
	}
}

bool Tile::getIsMine() {
	return isMine;
}

void Tile::setIsMine(bool i) {
	isMine = i;
	return;
}

void Tile::setAdjMines(int n) {
	set_label(to_string(n));
	set_label_color(minesToColor.at(n));
}