#ifndef ENTITY_H
#define ENTITY_H

#include "../vec2.h"

class Entity {
public:
	vec2 pos;
	
public:
	Entity(vec2 pos = vec2());
};

#endif