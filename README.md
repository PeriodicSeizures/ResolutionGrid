# ResolutionGrid
Semi efficient, simple alternative to quadtree

---
## Purpose
I was developing a 2d platformer from scratch, and starting to implement entity-entity collision detection. I thought ahead of time, knowing if that each entity checked against every other entity would be fast for a few entities, but start taking O(n*(n-1)) iterations to check each entity, where n is the total number of entities (I could be wrong). For a small number of 20 entities, is would take 20\*19 = **380 iterations**. I had the idea to use a quadtree, but as I searched, I found out that there aren't any simple, easy to use implementations for one in c++. Through more researching, I can store entities in a list, then have a rough grid, a fine grid, and an even finer grid of booleans, that stored whether an entity is present in that area. A final array of nodes, holding the index along with other data can be used to grab a few entities at a time that are close to each other (or something like that).

## Example
```cpp
#include <iostream>
#include "resgrid/resolutiongrid.h"
#include "entity/entity.h"
#include "lib/fix.h"

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

// user defined method conversion method for 'world position' --> 'grid index'
uint16_t worldToGridIndex(Entity *e) {
	return worldToGridIndex(e->pos);
}

int main(int argc, char **argv) {
	
  // creates a grid that holds Entity*, with method worldToGridIndex since there is no other easy way to get the position of the element
	auto grid = ResolutionGrid<Entity*, worldToGridIndex>();

	vec2 pos0 = { 4.6f, 3.9f };
	vec2 pos1 = { -14.5f, 504.6f };
	vec2 pos2 = { 317.9f, -59.3f };

	Entity e0(pos0);
	Entity e1(pos1);
	Entity e2(pos2);

	// insert test
	grid.insert(&e0);
	grid.insert(&e1);
	grid.insert(&e2);

  // buffer of found entities
	Entity **ret_buffer = new Entity*[3];
	uint16_t ret_end = 0;

  // a test to get all entities in the coarse grid at pos1 (might be a bug with how entities are retrieved, but it seems to work)
	grid.getCoarseElements(worldToGridIndex(pos1), ret_buffer, ret_end);
	
	printBuffer(ret_buffer, ret_end);
	
  // free
	delete[] ret_buffer;

	return 0;
}
```

## Usage
Create a ResolutionGrid with the following:
```cpp
auto grid = ResolutionGrid<element, worldToGridIndexMethod>();
```

Add elements with:
```cpp
grid.insert(element);
```

And retrieve elements with:
```cpp
// where arg0 is the converted index, ret_buffer being the array to modify, and ret_end being a referenced int for index to start at
grid.getCoarseElements(worldToGridIndex(pos1), ret_buffer, ret_end);
```

Finally, elements can have their positions updated with:
```cpp
grid.refresh();
```
