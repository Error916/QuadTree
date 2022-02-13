#include <stdio.h>

#include "quadtree.h"

int main(int argc, char **argv){
	(void) argc;
	(void) argv;

	XY center = XYconstruct(5, 5);
	AABB rect = AABBconstruct(center, 5);
	QuadTree *qt = QTconstruct(rect);

	for(int i = 0; i < 6; ++i){
		XY p = XYconstruct(i, 0);
		QTinsert(qt, &p);
	}

	QTqueryRange(qt, &rect);

	QTFree(qt);

	return 0;
}
