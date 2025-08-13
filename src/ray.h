#ifndef RAY_INCLUDED
#define RAY_INCLUDED

#include <raylib.h>

Vector2 get_ray(Vector2 pos, Vector2 ray_dir, int* map, int map_w, int map_h, int* horiz_hit);

#endif // RAY_INCLUDED