#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#define MAP_X	32				         // size of map along x-axis
#define MAP_Z	32				         // size of map along z-axis
#define MAP_SCALE	20.0f		         // the scale of the terrain map

#include "Util.h"

class Terrain {
public:
	void initTerrain();
	void renderTerrain();

private:
	float terrain[MAP_X][MAP_Z][3];
	GLUint texture;
};

#endif // __TERRAIN_H__
