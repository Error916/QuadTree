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

	if(point->x >= w && point->x <= e && point->y >= n && point->y <= s){
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

QuadTree *QTconstruct(AABB boundary){
	QuadTree *qt = malloc(sizeof(QuadTree));
	qt->boundary = boundary;
	qt->points_size = 0;
	qt->nw = NULL;
	qt->ne = NULL;
	qt->sw = NULL;
	qt->se = NULL;
	return qt;
}

void QTFree(QuadTree *qt){
	free(qt);

	if(qt->nw == NULL)
		return;

	QTFree(qt->nw);
	QTFree(qt->ne);
	QTFree(qt->sw);
	QTFree(qt->se);
}

uint8_t QTinsert(QuadTree *qt, XY *point){
	if(!AABBcontainPoint(&qt->boundary, point))
		return 0;

	if(qt->points_size < QT_NODE_CAP && qt->nw == NULL){
		qt->points[qt->points_size++] = *point;
		return 1;
	}

	if(qt->nw == NULL)
		QTsubdivide(qt);

	if(QTinsert(qt->nw, point)) return 1;
	if(QTinsert(qt->ne, point)) return 1;
	if(QTinsert(qt->sw, point)) return 1;
	if(QTinsert(qt->se, point)) return 1;

	return 0;
}

void QTsubdivide(QuadTree *qt){
	qt->nw = QTconstruct(AABBconstruct(XYconstruct(qt->boundary.center.x - qt->boundary.halfDimention / 2, qt->boundary.center.y - qt->boundary.halfDimention / 2 ), qt->boundary.halfDimention / 2));
	qt->ne = QTconstruct(AABBconstruct(XYconstruct(qt->boundary.center.x + qt->boundary.halfDimention / 2, qt->boundary.center.y - qt->boundary.halfDimention / 2 ), qt->boundary.halfDimention / 2));
	qt->sw = QTconstruct(AABBconstruct(XYconstruct(qt->boundary.center.x - qt->boundary.halfDimention / 2, qt->boundary.center.y + qt->boundary.halfDimention / 2 ), qt->boundary.halfDimention / 2));
	qt->se = QTconstruct(AABBconstruct(XYconstruct(qt->boundary.center.x + qt->boundary.halfDimention / 2, qt->boundary.center.y + qt->boundary.halfDimention / 2 ), qt->boundary.halfDimention / 2));
}

// TODO: make return a array of points
void QTqueryRange(QuadTree *qt, AABB *range){
	if(!AABBintersectAABB(&qt->boundary, range))
		return;

	for(size_t p = 0; p < qt->points_size; ++p){
		if(AABBcontainPoint(range, &qt->points[p]))
			printf("Punto x: %f y: %f\n", qt->points[p].x, qt->points[p].y);
	}

	if(qt->nw == NULL)
		return;

	QTqueryRange(qt->nw, range);
	QTqueryRange(qt->ne, range);
	QTqueryRange(qt->sw, range);
	QTqueryRange(qt->se, range);
}
