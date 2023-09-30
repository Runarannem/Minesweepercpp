#include "MinesweeperWindow.h"

int main()
{
	srand(time(NULL));
	Fl::background(200, 200, 200);
	constexpr int width = 10;
	constexpr int height = 10;
	constexpr int mines = 4;

	Point startPoint{ 200,300 };
	MinesweeperWindow mw{startPoint.x, startPoint.y, width, height, mines, "Minesweeper" };
	return gui_main();
}
