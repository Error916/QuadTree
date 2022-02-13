#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define QT_NODE_CAP 4

// Cordinate object
typedef struct {
	float x;
	float y;
} XY;

// Axis-aligned box
typedef struct {
	XY center;
	float halfDimention;
} AABB;

typedef struct quadtree {
	AABB boundary;
	XY points[QT_NODE_CAP];
	size_t points_size;

	struct quadtree *nw;
	struct quadtree *ne;
	struct quadtree *sw;
	struct quadtree *se;
} QuadTree;

XY XYconstruct(float x, float y);

AABB AABBconstruct(XY center, float halfDimention);
uint8_t AABBcontainPoint(AABB *rect, XY *point);
uint8_t AABBintersectAABB(AABB *rectA, AABB *rectB);

QuadTree QTconstruct(AABB boundary);
uint8_t insert(QuadTree *qt, XY *point);
void subdivide(QuadTree *qt);
void queryRange(QuadTree *qt, AABB *range);

#endif //QUADTREE_H_

#ifdef QUAD_IMPLEMENTATION
#endif //QUAD_IMPLEMENTATION
