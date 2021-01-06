#ifndef ENTITY_H
#define ENTITY_H

struct vec2 {
	float x, y;
};

class Entity {
public:
	vec2 pos;
	
public:
	Entity(vec2 pos = {0, 0});
};

#endif