#include <raylib.h>
#include "raymath.h"
#include "ray.h"

#define WINDOW_W 960
#define WINDOW_H 640
#define SCREEN_W 240
#define SCREEN_H 160

#define MAP_W 40
#define MAP_H 20
#define TILE_SIZE 24

Vector2 pos = { 0 };
Vector2 dir = { 0 };
Vector2 plane = { 0 };
Vector2 rays[SCREEN_W] = { 0 };
int horiz_hits[SCREEN_W] = { 0 };

static const int map[] = {
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 0, 0, 0, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 0, 0, 0, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void update_rays(void);
void draw_rays(void);
void draw_player(void);
void draw_map(void);

int main(void) {
	InitWindow(WINDOW_W, WINDOW_H, "Raylib Raycaster");
	SetTargetFPS(30);

	pos.x = 2.5f;
	pos.y = 1.5f;
	dir.y = -1.0f;
	plane.x = 0.66f;

	while ( !WindowShouldClose() ) {
		update_rays();
		
		int right = (int)IsKeyDown(KEY_RIGHT);
		int left = (int)IsKeyDown(KEY_LEFT);
		int up = (int)IsKeyDown(KEY_UP);
		int down = (int)IsKeyDown(KEY_DOWN);
		
		dir = Vector2Rotate(dir, (right - left) * GetFrameTime());
		plane = Vector2Rotate(plane, (right - left) * GetFrameTime());
		
		Vector2 vel = Vector2Scale(dir, (up - down));
		vel = Vector2Normalize(vel);
		vel = Vector2Scale(dir, (up - down) * 4 * GetFrameTime());
		/* Check x collision */
		if ( map[(int)(pos.x + vel.x) + (int)pos.y * MAP_W] != 0 ) vel.x = 0;
		/* Check y collision */
		if ( map[(int)pos.x + (int)(pos.y + vel.y) * MAP_W] != 0 ) vel.y = 0;
		pos = Vector2Add(pos, vel);

		BeginDrawing();
		ClearBackground(BLACK);
		
		draw_map();
		draw_rays();
		draw_player();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void update_rays(void) {
	for ( int i = 0; i < SCREEN_W; i++ ) {
		float x = 2 * i / (float)SCREEN_W - 1;
		Vector2 ray_dir = Vector2Add(dir, Vector2Scale(plane, x));
		rays[i] = get_ray(pos, ray_dir, map, MAP_W, MAP_H, &horiz_hits[i]);
	}
}

void draw_rays(void) {
	for ( int i = 0; i < SCREEN_W; i++ ) {
		DrawLineEx(Vector2Scale(pos, TILE_SIZE), Vector2Scale(Vector2Add(pos, rays[i]), TILE_SIZE), 1, GREEN);
	}
}

void draw_player(void) {
	/* Draw Player */
	DrawCircleV(Vector2Scale(pos, TILE_SIZE), 4.0f, BLUE);
	/* Draw Direction */
	DrawLineEx(Vector2Scale(pos, TILE_SIZE), Vector2Scale(Vector2Add(pos, dir), TILE_SIZE), 1, RED);
	/* Draw Plane */
	DrawLineEx(Vector2Scale(Vector2Add(pos, dir), TILE_SIZE), Vector2Scale(Vector2Add(Vector2Add(pos, dir), plane), TILE_SIZE), 1, ORANGE);
	DrawLineEx(Vector2Scale(Vector2Add(pos, dir), TILE_SIZE), Vector2Scale(Vector2Add(Vector2Add(pos, dir), Vector2Scale(plane, -1)), TILE_SIZE), 1, ORANGE);
}

void draw_map(void) {
	for ( int x = 0; x < MAP_W; x++ ) {
		for ( int y = 0; y < MAP_H; y++ ) {
			if ( map[x + y * MAP_W] == 0 ) continue;
			DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, WHITE);
		}
	}
}