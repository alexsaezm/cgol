#include "raylib.h"

enum { ROWS = 50, COLS = 50, CELL_SIZE = 10 };

// Because the evaluation of a generation happens in one step, I think is easier
// to use a swapping mechanism and keep two grids all the time. We do the
// calculations using now information but perform the change in next. And then
// swap them once the loop is over before painting.
int now[ROWS][COLS] = {0};
int next[ROWS][COLS] = {0};

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
		for (int y = 0; y <= 1; y++) {
			// the real coordinates
			int r = row + x;
			int c = col + y;

			// out of bounds check
			if (r >= 0 && r <= ROWS && c >= 0 && c <= COLS) {
				alive += now[r][c];
			}
		}
	}
	return alive;
}

void update(void) {
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			int alive = alive_in_the_neighborhood(row, col);
		}
	}
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
	// Seed some initial live cells
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			now[r][c] = (GetRandomValue(0, 4) == 0) ? 1 : 0;
		}
	}
	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_G)) {
			showGrid = !showGrid;
		}

		update();
		// TODO go back to the normal size of cells
		// draw(showGrid);
		//

		// This is way simplier right now for paiting and seeing if it works
		BeginDrawing();
		ClearBackground(BLACK);
		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLS; c++) {
				if (now[r][c] == 1) {
					DrawRectangle(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE - 1,
								  CELL_SIZE - 1, RAYWHITE);
				}
			}
		}
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
