#include <iostream>
#include <cassert>
#include "../resolutiongrid/resolutiongrid.h"
#include "entity/entity.h"

//#define NDEBUG
//using namespace 
using namespace resolutiongrid;

void printBuffer(Entity **buf, unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		printf("%.02f, %.02f\n", (float)buf[i]->pos.x, (float)buf[i]->pos.y);
	}
}

uint16_t worldToGridIndex(vec2 pos) {
	int16_t _x = (int)pos.x / WORLD_SCALE;
	int16_t _y = (int)pos.y / WORLD_SCALE;
	_x += (int)(GRID_WIDTH / 2);
	_y += (int)(GRID_WIDTH / 2);
	
	assert((_x >= 0 && _y >= 0) && "_x or _y are not unsigned in worldToGridIndex()");
	
	uint16_t index = (uint16_t)(_y * GRID_WIDTH + _x);
	
	assert((index < 16384) && "index was greater/equal to than 16384");
	
	return index;
}

uint16_t worldToGridIndex(Entity *e) {
	return worldToGridIndex(e->pos);
}

int main(int argc, char **argv) {
	vec2 pos0 = { 4.6f, 3.9f };
	vec2 pos1 = { -14.5f, 504.6f };
	vec2 pos2 = { 317.9f, -59.3f };

	Entity e0(pos0);
	Entity e1(pos1);
	Entity e2(pos2);

	// create grid
	auto grid = ResolutionGrid<Entity*, worldToGridIndex>();

	// insert test
	grid.insert(&e0);
	grid.insert(&e1);
	grid.insert(&e2);

	// retrieval test
	Entity **ret_buffer = new Entity*[3];
	uint16_t ret_end = 0;

	grid.getCoarseElements(worldToGridIndex(pos1), ret_buffer, ret_end);
	
	printBuffer(ret_buffer, ret_end);
	
	delete[] ret_buffer;

	return 0;
}