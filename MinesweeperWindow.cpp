#include "MinesweeperWindow.h"

MinesweeperWindow::MinesweeperWindow(int x, int y, int width, int height, int mines, const string &title) : 
	// Initialiser medlemsvariabler, bruker konstruktoren til AnimationWindow-klassen
	AnimationWindow{x, y, width * cellSize, (height + 1) * cellSize, title},
	width{width}, height{height}, mines{mines}, mineCount{mines}
{
	bombCount.value(to_string(mineCount).c_str());
	add(bombCount);
	// Legg til alle tiles i vinduet
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			tiles.emplace_back(new Tile{ Point{j * cellSize, i * cellSize}, cellSize});
			tiles.back()->callback(cb_click, this);
			add(tiles.back().get()); 
		}
	}

	// Legger til miner paa tilfeldige posisjoner
	for (int i = 0; i < mines; i++) {
		bool mineIsSet = false;
		while (!mineIsSet) {
			int pos = rand() % tiles.size();
			if (tiles[pos]->getIsMine() == false) {
				tiles[pos]->setIsMine(true);
				mineIsSet = true;
			}
		}
	}

	// Fjern window reskalering
	resizable(nullptr);
	size_range(x_max(), y_max(), x_max(), y_max());
}

vector<Point> MinesweeperWindow::adjacentPoints(Point xy) const {
	vector<Point> points;
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			if (di == 0 && dj == 0) {
				continue;
			}

			Point neighbour{ xy.x + di * cellSize,xy.y + dj * cellSize };
			if (inRange(neighbour)) {
				points.push_back(neighbour);
			}
		}
	}

	return points;
}

void MinesweeperWindow::openTile(Point xy) {
	shared_ptr<Tile> &tile = at(xy);
	Cell state = tile->getState();
	if (state == Cell::closed) {
		tile->open();
		if (tile->getIsMine() == false) {
			int count = countMines(adjacentPoints(xy));
			if (count > 0) {
				tile->setAdjMines(count);
				return;
			} else if (count == 0) {
				for (int i = 0; i < adjacentPoints(xy).size(); i++) {
					if (at(adjacentPoints(xy).at(i))->getState() == Cell::closed) {
						openTile(adjacentPoints(xy).at(i));
					}
				}
			}
		} else if (tile->getIsMine() == true) {
			loss = true;
		}
	}
}

void MinesweeperWindow::flagTile(Point xy) {
	shared_ptr<Tile> &tile = at(xy);
	Cell state = tile->getState();
	if (state == Cell::closed && state != Cell::open) {
		tile->flag();
		mineCount--;
	} else if (state == Cell::flagged) {
		tile->flag();
		mineCount++;
	}
	bombCount.value(to_string(mineCount).c_str());
}

//Kaller openTile ved venstreklikk og flagTile ved hoyreklikk/trykke med to fingre paa mac
void MinesweeperWindow::click()
{
	Point xy{Fl::event_x(), Fl::event_y()};

	MouseButton mb = static_cast<MouseButton>(Fl::event_button());

	if (!inRange(xy)) {
		return;
	}

	switch (mb) {
	case MouseButton::left:
		openTile(xy);
		WinOrLoss();
		win = hasWon();
		WinOrLoss();
		break;
	case MouseButton::right:
		flagTile(xy);
		break;
	}
}


int MinesweeperWindow::countMines(vector<Point> coords) const {
	int count = 0;
	for (int i = 0; i < coords.size(); i++) {
		if (at(coords[i])->getIsMine()) {
			count++;
		}
	}
	return count;
}

bool MinesweeperWindow::hasWon() {
	int unopenedTiles = 0;
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles.at(i)->getState() != Cell::open) {
			unopenedTiles++;
		}
	}
	if (unopenedTiles == mines) {
		
		return true;
	} else {
		return false;
	}
}

void MinesweeperWindow::WinOrLoss() {
	if (win == true) {
		winOrLose.value("You won!");
		add(winOrLose);
		for (int i = 0; i < tiles.size(); i++) {
			if (tiles.at(i)->getIsMine()) {
				if (tiles.at(i)->getState() != Cell::flagged) {
					tiles.at(i)->flag();
				}
			}
			if (tiles.at(i)->getState() == Cell::closed && !tiles.at(i)->getIsMine()) {
				tiles.at(i)->open();
			}
		}
		add(quit);
		quit.callback(cb_quit, this);
		add(restart);
		restart.callback(cb_restart, this);
	} else if (loss == true) {
		winOrLose.value("You lost!");
		add(winOrLose);
		for (int i = 0; i < tiles.size(); i++) {
			if (tiles.at(i)->getIsMine()) {
				if (tiles.at(i)->getState() == Cell::flagged) {
					tiles.at(i)->flag();
				}
				tiles.at(i)->open();
			}
		}
		add(quit);
		quit.callback(cb_quit, this);
		add(restart);
		restart.callback(cb_restart, this);
	}
}

int MinesweeperWindow::restartGame() {
	Fl::background(200, 200, 200);
	constexpr int width = 10;
	constexpr int height = 10;
	constexpr int mines = 4;

	Point startPoint{ 200,300 };
	MinesweeperWindow mw{startPoint.x, startPoint.y, width, height, mines, "Minesweeper" };
	return gui_main();
}