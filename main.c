#include "raylib.h"

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "cgol");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    const char *text = "cgol";

    int fontSize = 40;
    int textWidth = MeasureText(text, fontSize);

    int x = (GetScreenWidth() - textWidth) / 2;
    int y = (GetScreenHeight() - fontSize) / 2;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(text, x, y, fontSize, WHITE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
