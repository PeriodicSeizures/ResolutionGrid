# ResolutionGrid
Semi efficient, simple alternative to quadtree

---
## Purpose
I was developing a 2d platformer from scratch, and starting to implement entity-entity collision detection. I thought ahead of time, knowing if that each entity checked against every other entity would be fast for a few entities, but start taking O(n*(n-1)) iterations to check each entity, where n is the total number of entities (I could be wrong). For a small number of 20 entities, is would take 20\*19 = **380 iterations**. I had the idea to use a quadtree, but as I searched, I found out that there aren't any simple, easy to use implementations for one in c++. Through more researching, I can store entities in a list, then have a rough grid, a fine grid, and an even finer grid of booleans, that stored whether an entity is present in that area. A final array of nodes, holding the index along with other data can be used to grab a few entities at a time that are close to each other (or something like that).

## Example
located under /test

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

There is currently no way to remove an element after is has been inserted, only clearing the entire grid:
```cpp
grid.clear(fastClear)
```
