#ifndef RESOLUTIONGRID_H
#define RESOLUTIONGRID_H

#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "fastunorderedlist.h"



//#define NDEBUG
namespace resolutiongrid {
	//static constexpr int i = 0;
	static constexpr uint8_t BASE_SIZE = 8;
	static constexpr uint8_t BASE_MULTIPLIER = 4;
	static constexpr uint8_t WORLD_SCALE = 20; // from nodes size * 10 on each axis

	static constexpr uint16_t ROUGH_WIDTH = BASE_SIZE;
	static constexpr uint16_t COARSE_WIDTH = ROUGH_WIDTH * BASE_MULTIPLIER;
	static constexpr uint16_t GRID_WIDTH = COARSE_WIDTH * BASE_MULTIPLIER;

	static constexpr uint16_t ROUGH_SIZE = ROUGH_WIDTH * ROUGH_WIDTH;
	static constexpr uint16_t COARSE_SIZE = COARSE_WIDTH * COARSE_WIDTH;
	static constexpr uint16_t GRID_SIZE = GRID_WIDTH * GRID_WIDTH;



	template<typename T, uint16_t(*worldToGridIndexMethod)(T)> class ResolutionGrid {
	private:

		struct ElementWrapper {

			// contains entity and prev node index
			T element;

			// POSITION UPDATES
			uint16_t prev_grid_index; // aka prev position on grid

			// index in elements (self index)
			uint16_t element_index;

			//index in the list of 'element_grid'
			// ie:			element_grid[i].erase(eg_index)
			uint16_t element_grid_insert_index;
		};


		//uint32_t *rough;
		//uint32_t *coarse;

		bool *rough;
		bool *coarse;

		//Node *nodes;
		//FastUnorderedList<uint16_t> *element_grid;
		FastUnorderedList<uint16_t> element_grid[GRID_SIZE]; // contains all indexes to elements
		FastUnorderedList<ElementWrapper> elements; // contains the elements referenced by element_grid above

		//static constexpr uint16_t worldToGridIndex(const vec2& pos) {
		//	int16_t _x = (int)pos.x / WORLD_SCALE;
		//	int16_t _y = (int)pos.y / WORLD_SCALE;
		//	_x += (int)(GRID_WIDTH / 2);
		//	_y += (int)(GRID_WIDTH / 2);
		//
		//	assert((_x >= 0 && _y >= 0) && "_x or _y are not unsigned in worldToGridIndex()");
		//
		//	uint16_t index = (uint16_t)(_y * GRID_WIDTH + _x);
		//
		//	assert((index < 16384) && "index was greater/equal to than 16384");
		//
		//	return index;
		//}

		static constexpr uint16_t gridCoordinateToGridIndex(uint8_t x, uint8_t y) {
			return y * GRID_WIDTH + x;
		}

		static constexpr void gridCoordinate(uint16_t grid_index, uint8_t &ret_x, uint8_t &ret_y) {
			ret_x = grid_index % GRID_WIDTH;
			ret_y = grid_index / GRID_WIDTH;
		}

		static constexpr uint16_t getRoughIndex(uint16_t grid_index) {
			return grid_index / (GRID_SIZE / ROUGH_SIZE);
		}

		static constexpr uint16_t getCoarseIndex(uint16_t grid_index) {
			return grid_index / (GRID_SIZE / COARSE_SIZE);
		}

	public:
		ResolutionGrid() {
			// 64 bits; 8x8 elements
			this->rough = new bool[ROUGH_SIZE];

			// 1024 bits; 32x32 elements
			this->coarse = new bool[COARSE_SIZE];
		}

		~ResolutionGrid() {
			delete[] this->rough;
			delete[] this->coarse;
		}

		void insert(const T &element) {
			// the number resized especially for nodes
			uint16_t grid_index = worldToGridIndexMethod(element);

			rough[getRoughIndex(grid_index)] = 1;
			coarse[getCoarseIndex(grid_index)] = 1;

			/*

						  DEBUG TESTS

			*/

			///uint8_t x, y;
			///gridCoordinate(grid_index, x, y);
			///printf("%u %u\n", x, y);


			/*

				.........END OF DEBUG........

			*/

			// set grid data
			uint16_t element_grid_insert_index = element_grid[grid_index].insert(elements.size());

			// add element to single global
			elements.insert({ T(element), grid_index, static_cast<uint16_t>(elements.size()), element_grid_insert_index });

		}

		void clear(bool fast) {
			if (fast) {
				for (unsigned int i = 0; i < GRID_SIZE; i++) {
					element_grid[i].fastClear();
				}
				elements.fastClear();
			}
			else {
				for (unsigned int i = 0; i < GRID_SIZE; i++) {
					element_grid[i].clear();
				}
				elements.clear();
			}
		}

		void update_grid() {
			// iterate every entity, checking for changes in current position (converted to an node index) and prevIndex
			for (uint16_t i = 0; i < elements.size(); i++) {
				//if
				ElementWrapper& element = elements[i];

				/// if null reference
				///if (!e.entity) {
				///
				///	// remove from grid
				///	element_grid[e.prev_grid_index].erase(e.element_grid_insert_index);
				///
				///	// then 
				///
				///	// remove from elements
				///	elements.erase(e.prev_grid_index);
				///	continue;
				///}

				// if entity has moved outside of grid[index]
				const uint16_t current_grid_index = worldToGridIndexMethod(element);
				if (element.prev_grid_index != current_grid_index) {

					rough[getRoughIndex(element.prev_grid_index)] = 0;
					coarse[getCoarseIndex(element.prev_grid_index)] = 0;


					rough[getRoughIndex(current_grid_index)] = 1;
					coarse[getCoarseIndex(current_grid_index)] = 1;

					// insert the new
					element_grid[current_grid_index].insert(element.element_index);

					// erase the old
					element_grid[element.prev_grid_index].erase(element.element_grid_insert_index);



					// set the new index
					element.prev_grid_index = current_grid_index;
				}
			}

		}

		//std::vector<Entity*> getNearbyEntities(vec2 center, int16_t r); // finds entities in a real world range
		void getCoarseEntities(uint16_t grid_index, T *ret_buffer, uint16_t &ret_i) { // finds entities in a real world range
				// return all entities in v-ish area
			//uint16_t grid_index = this->worldToGridIndex(v);

			if (!coarse[getCoarseIndex(grid_index)]) return; // if no entities at coarse resolution return

			// else get entities at node resolution
			uint8_t _x, _y;
			gridCoordinate(grid_index, _x, _y);

			for (uint16_t y = _y; y < _y + COARSE_WIDTH; y++) {
				for (uint16_t x = _x; x < _x + COARSE_WIDTH; x++) {
					for (unsigned int i = 0; i < element_grid[y*GRID_WIDTH + x].size(); i++) {
						unsigned int e_index = element_grid[y*GRID_WIDTH + x][i];
						ret_buffer[ret_i++] = elements[e_index].element;
					}

				}
			}
		}
	};
}

#endif