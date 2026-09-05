#include "raylib.h"

enum { ROWS = 50, COLS = 50 };

void update(void) {
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
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

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_G)) {
			showGrid = !showGrid;
		}

		update();
		draw(showGrid);
	}

	CloseWindow();

	return 0;
}
