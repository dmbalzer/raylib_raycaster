#include <raylib.h>

int main(void) {
	InitWindow(960,640,"Raylib Raycaster");
	SetTargetFPS(30);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
