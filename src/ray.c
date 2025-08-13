#include "ray.h"
#include <raymath.h>

Vector2 get_ray(Vector2 pos, Vector2 ray_dir, const int* map, int map_w, int map_h, int* horiz_hit) {
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
		if ( x >= map_w ) x = map_w - 1;
		if ( x < 0 ) x = 0;
		int y = ray_dir.y < 0 ? (int)(pos.y + hray.y) - 1 : (int)(pos.y + hray.y);
		if ( y >= map_h ) y = map_h - 1;
		if ( y < 0 ) y = 0;
		if ( map[ x + y * map_w ] != 0 ) {
			
			h_map_no = map[ x + y * map_w ];
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
		if ( x >= map_w ) x = map_w - 1;
		if ( x < 0 ) x = 0;
		int y = (int)(pos.y + vray.y);
		if ( y >= map_h ) y = map_h - 1;
		if ( y < 0 ) y = 0;
		if ( map[ x + y * map_w ] != 0 ) {
			
			v_map_no = map[ x + y * map_w ];
			break;
		}
		vray = Vector2Add(vray, vstep);
	}

    *horiz_hit = (Vector2Length(hray) < Vector2Length(vray)) ? 1 : 0;
	return (Vector2Length(hray) < Vector2Length(vray)) ? hray : vray;
}