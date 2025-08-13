#ifndef RAY_INCLUDED
#define RAY_INCLUDED

typedef struct _ray Ray;
struct _ray {
	float x;		// Vector component x
	float y;		// Vector component y
	float x_perp;	// Perpendicular Vector component x
	float y_perp;	// Perpendicular Vector component y
	int horiz_hit;	// Was hit on horizontal side of wall
	int map_hit_idx;// Map index of hit
};

Ray ray_cast(float xpos, float ypos, float xdir, float ydir, int* map, int map_w, int map_h);

#endif // RAY_INCLUDED
