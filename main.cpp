#include <curses.h>

#include <fstream>

using namespace std;

void conway(bool** world, int height, int width);

int countNeighbors(bool** world, int y, int x, int height, int width);

int main(int argc, char** argv) {
	int ch = '\0';
	MEVENT event;

	initscr();

	noecho();
	cbreak();
	keypad(stdscr, true);
	mousemask(BUTTON1_CLICKED, nullptr);

	int height = LINES, width = COLS / 2;
	bool** world = new bool*[height];

	for (int i = 0; i < height; i++) {
		world[i] = new bool[width];

		for (int j = 0; j < width; j++) {
			world[i][j] = false;
		}
	}

	// world[1][25] = world[2][23] = world[2][25] = world[3][13] = world[3][14] = world[3][21] = world[3][22] = world[3][35] = world[3][36] = world[4][12] =
	// 	world[4][16] = world[4][21] = world[4][22] = world[4][35] = world[4][36] = world[5][1] = world[5][2] = world[5][11] = world[5][17] = world[5][21] =
	// 		world[5][22] = world[6][1] = world[6][2] = world[6][11] = world[6][15] = world[6][17] = world[6][18] = world[6][23] = world[6][25] =
	// 			world[7][11] = world[7][17] = world[7][25] = world[8][12] = world[8][16] = world[9][13] = world[9][14] = true;

	if (argc > 1) {
		fstream in(argv[1]);

		int row = 0;
		char fchar;
		for (int i = 0; in.read(&fchar, 1); i++) {
			if (fchar == '\n') {
				row++;
				i = 0;
			} else {
				world[row][i] = fchar == 'a';
			}
		}

		in.close();
	}

	do {
		switch (ch) {
			case 'n':
				conway(world, height, width);
				break;
			case KEY_MOUSE:
				if (getmouse(&event) == OK) {
					if (event.bstate & BUTTON1_CLICKED) {
						if (event.y < height && event.x / 2 < width) {
							world[event.y][event.x / 2] = !world[event.y][event.x / 2];
						}
					}
				}
				break;
			case '\0':
				break;
			default:
				goto skip;
		}

		clear();
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (world[i][j]) {
					addch('\xFE');
					addch('\xFE');
				} else {
					addch(' ');
					addch(' ');
				}
			}

			if (width * 2 < COLS) {
				addch(' ');
			}
		}

		refresh();
	skip:
		void(0);
	} while ((ch = getch()) != 'q');
	endwin();

	for (int i = 0; i < height; i++) {
		delete[] world[i];
	}
	delete[] world;

	return 0;
}

void conway(bool** world, int height, int width) {
	bool** newWorld = new bool*[height];

	for (int i = 0; i < height; i++) {
		newWorld[i] = new bool[width];

		for (int j = 0; j < width; j++) {
			newWorld[i][j] = false;
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int neighbors = countNeighbors(world, i, j, height, width);

			if (world[i][j]) {
				if (neighbors < 2) {
					newWorld[i][j] = false;
				} else if (neighbors > 3) {
					newWorld[i][j] = false;
				} else {
					newWorld[i][j] = true;
				}
			} else if (neighbors == 3) {
				newWorld[i][j] = true;
			}
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			world[i][j] = newWorld[i][j];
		}
	}

	for (int i = 0; i < height; i++) {
		delete[] newWorld[i];
	}
	delete[] newWorld;
}

int countNeighbors(bool** world, int y, int x, int height, int width) {
	int aliveCount = 0;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) {
				continue;
			}

			if (world[(y + i + height) % height][(x + j + width) % width]) {
				aliveCount++;
			}
		}
	}

	return aliveCount;
}