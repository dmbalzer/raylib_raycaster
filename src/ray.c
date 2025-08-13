#include "ray.h"
#include "defs.h"
#include <SDL3/SDL.h>

/* sdl.c */
extern SDL_Renderer* renderer;

/* player.c */
extern Vector2 pos;
extern Vector2 dir;
extern Vector2 plane;

/* map.c */
extern const int map[];

static float screen_dist = 0.0f;

Vector2 rays[SCREEN_W] = { 0 };
bool ray_sides[SCREEN_W] = { 0 };
float wall_strip_heights[SCREEN_W] = { 0 };
int map_hit_nos[SCREEN_W] = { 0 };

static void _ray_cast(Vector2 ray_dir, Vector2* ray, bool* h_hit, int* map_hit_no) {
	/******************************************************
	 * Horizontal Side Checks
	 ******************************************************/
	/* Horizontal Side Vector*/
	Vector2 hside = { 0 };
	hside.y = ray_dir.y < 0 ? (int)pos.y - pos.y : (int)pos.y - pos.y + 1;
	hside.x = hside.y / tan(Vector2Angle((Vector2){1.0f, 0.0f}, ray_dir));

	/* Horizontal Step Vector*/
	Vector2 hstep = { 0 };
	hstep.y = ray_dir.y < 0 ? -1 : 1;
	hstep.x = hstep.y / tan(Vector2Angle((Vector2){1.0f, 0.0f}, ray_dir));

	bool hit = false;
	Vector2 hray = hside;
	int h_map_no = 0;
	while ( !hit ) {
		int x = (int)(pos.x + hray.x);
		if ( x >= MAP_W ) x = MAP_W - 1;
		if ( x < 0 ) x = 0;
		int y = ray_dir.y < 0 ? (int)(pos.y + hray.y) - 1 : (int)(pos.y + hray.y);
		if ( y >= MAP_H ) y = MAP_H - 1;
		if ( y < 0 ) y = 0;
		if ( map[ x + y * MAP_W ] != 0 ) {
			
			h_map_no = map[ x + y * MAP_W ];
			break;
		}
		hray = Vector2Add(hray, hstep);
	}

	/******************************************************
	 * Vertical Side Checks
	 ******************************************************/
	/* Vertical Side Vector*/
	Vector2 vside = { 0 };

	vside.x = ray_dir.x < 0 ? (int)pos.x - pos.x : (int)pos.x - pos.x + 1;
	vside.y = vside.x * tan(Vector2Angle((Vector2){1.0f, 0.0f}, ray_dir));

	/* Vertical Step Vector*/
	Vector2 vstep = { 0 };
	vstep.x = ray_dir.x < 0 ? -1 : 1;
	vstep.y = vstep.x * tanf(Vector2Angle((Vector2){1.0f, 0.0f}, ray_dir));

	hit = false;
	Vector2 vray = vside;
	int v_map_no = 0;
	while ( !hit ) {
		int x = ray_dir.x < 0 ? (int)(pos.x + vray.x) - 1: (int)(pos.x + vray.x);
		if ( x >= MAP_W ) x = MAP_W - 1;
		if ( x < 0 ) x = 0;
		int y = (int)(pos.y + vray.y);
		if ( y >= MAP_H ) y = MAP_H - 1;
		if ( y < 0 ) y = 0;
		if ( map[ x + y * MAP_W ] != 0 ) {
			
			v_map_no = map[ x + y * MAP_W ];
			break;
		}
		vray = Vector2Add(vray, vstep);
	}
	
	*h_hit = (Vector2Length(hray) < Vector2Length(vray));
	*ray = *h_hit ? hray : vray;
	*map_hit_no = *h_hit ? h_map_no : v_map_no;
}

float _get_perp_dist(Vector2 cam_plane, Vector2 ray, bool h_hit) {
	float angle = Vector2Angle(ray, dir);
	return Vector2Length(ray) * cosf(angle);
}

void ray_update(void) {
	if ( screen_dist == 0.0f ) {
		float half_fov = Vector2Angle(dir, Vector2Add(dir, plane));
		screen_dist = (SCREEN_W / 2) / tanf(half_fov);
	}

    for ( int i = 0; i < SCREEN_W; i++ ) {
		float x = 2 * i / (float)SCREEN_W - 1;
		Vector2 cam_plane = Vector2Scale(plane, x);
		Vector2 ray_dir = (Vector2){
			.x = dir.x + cam_plane.x,
			.y = dir.y + cam_plane.y
		};

        _ray_cast(ray_dir, &rays[i], &ray_sides[i], &map_hit_nos[i]);
		float perp_dist = _get_perp_dist(cam_plane, rays[i], ray_sides[i]);
		wall_strip_heights[i] = 1 / perp_dist * screen_dist;
    }
}

void ray_draw(void) {
	for ( int i = 0; i < SCREEN_W; i++ ) {

		/* Draw Ray */
		SDL_SetRenderDrawColor(renderer, GREEN);
		SDL_RenderLine(renderer,
			pos.x * TILE_SIZE,
			pos.y * TILE_SIZE,
			(pos.x + rays[i].x) * TILE_SIZE,
			(pos.y + rays[i].y) * TILE_SIZE);
	}
}
