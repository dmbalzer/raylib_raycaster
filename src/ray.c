#include "ray.h"
#include <math.h>

static float VectorAngle(float x1, float y1, float x2, float y2) {
    float result = 0.0f;

    float dot = x1*x2 + y1*y2;
    float det = x1*y2 - y1*x2;

    result = atan2f(det, dot);

    return result;
}

static float VectorLength(float x, float y) {
    float result = sqrtf((x*x) + (y*y));
    return result;
}

Ray ray_cast(float pos_x, float pos_y, float dir_x, float dir_y, int* map, int map_w, int map_h) {

	/******************************************************
	 * Horizontal Side Checks
	 ******************************************************/
	/* Calculate Horizontal Side Distances */
	float hside_y = dir_y < 0 ? (int)pos_y - pos_y : (int)pos_y - pos_y + 1;
	float hside_x = hside_y / tanf(VectorAngle(1.0f, 0.0f, dir_x, dir_y));

	/* Calculate Horizontal Step Distances */
	Vector2 hstep = { 0 };
	float hstep_y = dir_y < 0 ? -1 : 1;
	float hstep_x = hstep_y / tanf(VectorAngle(1.0f, 0.0f, dir_x, dir_y));

	int hit = 0;
	float hray_x = hside_y;
	float hray_y = hside_x;
	int h_map_hit_no = 0;

	while ( !hit ) {

		int x = (int)(pos_x + hray_x);
		int y = dir_y < 0 ? (int)(pos_y + hray_y) - 1 : (int)(pos_y + hray_y);

		if ( map[ x + y * map_w ] != 0 ) {
			
			h_map_hit_no = map[ x + y * map_w ];
			break;
		}

		hray_x += hstep_x;
		hray_y += hstep_y;

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

	Ray ray = { 0 };
	
	*h_hit = (Vector2Length(hray) < Vector2Length(vray));
	*ray = *h_hit ? hray : vray;
	*map_hit_no = *h_hit ? h_map_no : v_map_no;
}

// float _get_perp_dist(Vector2 cam_plane, Vector2 ray, bool h_hit) {
// 	float angle = Vector2Angle(ray, dir);
// 	return Vector2Length(ray) * cosf(angle);
// }
// 
// void ray_update(void) {
// 	if ( screen_dist == 0.0f ) {
// 		float half_fov = Vector2Angle(dir, Vector2Add(dir, plane));
// 		screen_dist = (SCREEN_W / 2) / tanf(half_fov);
// 	}
// 
//     for ( int i = 0; i < SCREEN_W; i++ ) {
// 		float x = 2 * i / (float)SCREEN_W - 1;
// 		Vector2 cam_plane = Vector2Scale(plane, x);
// 		Vector2 ray_dir = (Vector2){
// 			.x = dir.x + cam_plane.x,
// 			.y = dir.y + cam_plane.y
// 		};
// 
//         _ray_cast(ray_dir, &rays[i], &ray_sides[i], &map_hit_nos[i]);
// 		float perp_dist = _get_perp_dist(cam_plane, rays[i], ray_sides[i]);
// 		wall_strip_heights[i] = 1 / perp_dist * screen_dist;
//     }
// }
