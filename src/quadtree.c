#define _DEFAULT_SOURCE

#include "quadtree.h"

XY XYconstruct(float x, float y){
	XY point = {.x = x, .y = y};
	return point;
}

AABB AABBconstruct(XY center, float halfDimention){
	AABB rect = {.center = center, .halfDimention = halfDimention};
	return rect;
}

uint8_t AABBcontainPoint(AABB *rect, XY *point){
	float w = rect->center.x - rect->halfDimention;
	float e = rect->center.x + rect->halfDimention;
	float n = rect->center.y - rect->halfDimention;
	float s = rect->center.y + rect->halfDimention;

	if(point->x > w && point->x < e && point->y > n && point->y < s){
		return 1;
	}

	return 0;
}

uint8_t AABBintersectAABB(AABB *rectA, AABB *rectB){
	if(
		rectA->center.x > rectB->center.x + rectB->halfDimention ||
		rectA->center.x + rectA->halfDimention < rectB->center.x ||
		rectA->center.y > rectB->center.y + rectB->halfDimention ||
		rectA->center.y + rectA->halfDimention < rectB->center.y
	){
		return 0;
	}

	return 1;
}

QuadTree QTconstruct(AABB boundary){
	QuadTree qt = {.boundary = boundary, .points_size = 0, .nw = NULL, .ne = NULL, .sw = NULL, .se = NULL};
	return qt;
}

uint8_t insert(QuadTree *qt, XY *point){
	if(!AABBcontainPoint(&qt->boundary, point))
		return 0;

	if(qt->points_size < QT_NODE_CAP && qt->nw == NULL){
		qt->points[qt->points_size++] = *point;
		return 1;
	}

	if(qt->nw == NULL)
		subdivide(qt);

	if(insert(qt->nw, point)) return 1;
	if(insert(qt->ne, point)) return 1;
	if(insert(qt->sw, point)) return 1;
	if(insert(qt->se, point)) return 1;

	return 0;
}

// FIXME: make things permanent
void subdivide(QuadTree *qt){
	QuadTree nw = QTconstruct(AABBconstruct(XYconstruct(qt->boundary.center.x - qt->boundary.halfDimention / 2, qt->boundary.center.y - qt->boundary.halfDimention / 2 ), qt->boundary.halfDimention / 2));
	QuadTree ne = QTconstruct(AABBconstruct(XYconstruct(qt->boundary.center.x + qt->boundary.halfDimention / 2, qt->boundary.center.y - qt->boundary.halfDimention / 2 ), qt->boundary.halfDimention / 2));
	QuadTree sw = QTconstruct(AABBconstruct(XYconstruct(qt->boundary.center.x - qt->boundary.halfDimention / 2, qt->boundary.center.y + qt->boundary.halfDimention / 2 ), qt->boundary.halfDimention / 2));
	QuadTree se = QTconstruct(AABBconstruct(XYconstruct(qt->boundary.center.x + qt->boundary.halfDimention / 2, qt->boundary.center.y + qt->boundary.halfDimention / 2 ), qt->boundary.halfDimention / 2));
	qt->nw = &nw;
	qt->ne = &ne;
	qt->sw = &sw;
	qt->se = &se;
}

// TODO: make return a array of points
void queryRange(QuadTree *qt, AABB *range){
	if(!AABBintersectAABB(&qt->boundary, range))
		return;

	for(size_t p = 0; p < qt->points_size; ++p){
		if(AABBcontainPoint(range, &qt->points[p]))
			printf("Punto x: %f y: %f\n", qt->points[p].x, qt->points[p].y);
	}

	if(qt->nw == NULL)
		return;

	queryRange(qt->nw, range);
	queryRange(qt->ne, range);
	queryRange(qt->sw, range);
	queryRange(qt->se, range);
}
