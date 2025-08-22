#include <raylib.h>
#include <string>

int main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "raylib - basic window");
	SetTargetFPS(60);
	while ( !WindowShouldClose() ) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		std::string text = "Congrats! You created your first window!";
		Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), 20, 2);
		DrawText(text.c_str(), GetScreenWidth() / 2.0 - textSize.x / 2, GetScreenHeight() / 2.0 - textSize.y / 2, 20, RED);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
