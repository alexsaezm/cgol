#include "raylib.h"

enum { ROWS = 100, COLS = 100 };

// Because the evaluation of a generation happens in one step, I think is easier
// to use a swapping mechanism and keep two grids all the time. We do the
// calculations using now information but perform the change in next. And then
// swap them once the loop is over before painting.
int now[ROWS][COLS] = {0};
int next[ROWS][COLS] = {0};
int (*nowp)[COLS] = now;
int (*nextp)[COLS] = next;

/*
 * The position we get is the 0,0 so we need to evaluate how many alive cells
 * are in the neighborhood and return the number.
 *
 * [-1,-1] [-1,0] [-1,1]
 * [0,-1]  [0,0]  [0,1]
 * [1,-1]  [1,0]  [1,1]
 */
int alive_in_the_neighborhood(int row, int col) {
	int alive = 0;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0) {
				continue;
			}

			// the real coordinates
			int r = row + x;
			int c = col + y;

			// out of bounds check
			if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
				alive += nowp[r][c];
			}
		}
	}
	return alive;
}

void update(void) {
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			int alive = alive_in_the_neighborhood(row, col);
			int isAlive = nowp[row][col];
			nextp[row][col] = 0;
			// Any live cell with fewer than two live neighbours dies, as if
			// caused by underpopulation.
			if (isAlive && alive < 2) {
				nextp[row][col] = 0;
			}
			// Any live cell with two or three live neighbours lives on to the
			// next generation.
			if (isAlive && (alive == 2 || alive == 3)) {
				nextp[row][col] = 1;
			}
			// Any live cell with more than three live neighbours dies, as if by
			// overpopulation.
			if (isAlive && alive > 3) {
				nextp[row][col] = 0;
			}
			// Any dead cell with exactly three live neighbours becomes a live
			// cell, as if by reproduction.
			if (!isAlive && alive == 3) {
				nextp[row][col] = 1;
			}
		}
	}
	// Swap the grids!
	int (*t)[COLS] = nowp;
	nowp = nextp;
	nextp = t;
}

void draw(bool showGrid) {
	int width = GetScreenWidth();
	int height = GetScreenHeight();

	const int footerHeight = 40;
	const int fontSize = 20;

	int availableHeight = height - footerHeight;

	float cellWidth = (float)width / COLS;
	float cellHeight = (float)availableHeight / ROWS;
	float cellSize = cellWidth < cellHeight ? cellWidth : cellHeight;

	float gridWidth = COLS * cellSize;
	float gridHeight = ROWS * cellSize;

	float offsetX = (width - gridWidth) / 2.0f;
	float offsetY = (availableHeight - gridHeight) / 2.0f;

	BeginDrawing();
	ClearBackground(BLACK);

	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			if (nowp[row][col] == 1) {
				DrawRectangle((int)(offsetX + col * cellSize),
							  (int)(offsetY + row * cellSize), (int)cellSize,
							  (int)cellSize, RAYWHITE);
			}
		}
	}

	if (showGrid) {
		for (int row = 0; row <= ROWS; row++) {
			float y = offsetY + row * cellSize;

			DrawLine((int)offsetX, (int)y, (int)(offsetX + gridWidth), (int)y,
					 LIGHTGRAY);
		}

		for (int col = 0; col <= COLS; col++) {
			float x = offsetX + col * cellSize;

			DrawLine((int)x, (int)offsetY, (int)x, (int)(offsetY + gridHeight),
					 LIGHTGRAY);
		}
	}

	const char *legend = "[G] Show/hide grid";
	int legendWidth = MeasureText(legend, fontSize);

	DrawText(legend, (width - legendWidth) / 2,
			 height - footerHeight + (footerHeight - fontSize) / 2, fontSize,
			 LIGHTGRAY);

	EndDrawing();
}

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "gcol");
	MaximizeWindow();

	SetTargetFPS(60);

	bool showGrid = false;
	double nextUpdate = GetTime();
	// Seed some initial live cells
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			now[r][c] = (GetRandomValue(0, 10) == 0) ? 1 : 0;
		}
	}
	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_G)) {
			showGrid = !showGrid;
		}

		if (GetTime() >= nextUpdate) {
			update();
			nextUpdate = GetTime() + 0.1;
		}
		draw(showGrid);
	}

	CloseWindow();

	return 0;
}
