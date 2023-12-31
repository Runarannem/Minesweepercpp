#pragma once
#include "AnimationWindow.h"
#include "Tile.h"

enum class MouseButton { left = FL_LEFT_MOUSE, right = FL_RIGHT_MOUSE }; 

// Minesweeper GUI
class MinesweeperWindow : public AnimationWindow
{
public:
	bool win = false;
	bool loss = false;
	// storrelsen til hver tile
	static constexpr int cellSize = 30;
	MinesweeperWindow(int x, int y, int width, int height, int mines, const string& title);
private:
	int mineCount;
	Fl_Output winOrLose = Fl_Output(120, 300, 60, 30);
	Fl_Output bombCount = Fl_Output(140, 300, 20, 30);
	Fl_Button quit = Fl_Button(230, 300, 60, 30, "Quit");
	static void cb_quit(Fl_Widget*, void* pw) { static_cast<MinesweeperWindow*>(pw)->hide(); };
	Fl_Button restart = Fl_Button(10, 300, 60, 30, "Restart");
	static void cb_restart(Fl_Widget*, void* pw) { static_cast<MinesweeperWindow*>(pw)->hide(); static_cast<MinesweeperWindow*>(pw)->restartGame(); };

	const int width;		// Bredde i antall tiles
	const int height;		// Hoyde i antall tiles
	const int mines;		// Antall miner
	vector<shared_ptr<Tile>> tiles; // Vektor som inneholder alle tiles


	// hoyde og bredde i piksler
	int Height() const { return height * cellSize; } 
	int Width() const { return width * cellSize; }

	// Returnerer en vektor med nabotilene rundt en tile, der hver tile representeres som et punkt
	vector<Point> adjacentPoints(Point xy) const;
	// tell miner basert paa en liste tiles
	int countMines(vector<Point> coords) const;

	// Sjekker at et punkt er paa brettet
	bool inRange(Point xy) const { return xy.x >= 0 && xy.x< Width() && xy.y >= 0 && xy.y < Height(); }
	// Returnerer en tile gitt et punkt
	shared_ptr<Tile>& at(Point xy) { return tiles[xy.x / cellSize + (xy.y / cellSize) * width]; }
	const shared_ptr<Tile>& at(Point xy) const { return tiles[xy.x / cellSize + (xy.y / cellSize) * width]; }

    //aapne og flagge rute
	void openTile(Point xy);
	void flagTile(Point xy);

	bool hasWon();

	// callback funksjoner for de tile knappene
	static void cb_click(Fl_Widget*, void* pw) { static_cast<MinesweeperWindow*>(pw)->click(); };
	void click();
	
	void WinOrLoss();
	int restartGame();
};
