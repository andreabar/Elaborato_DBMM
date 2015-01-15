/*
 * BoundingBox.h
 *
 *  Created on: 13/gen/2015
 *      Author: federico
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

class BoundingBox {

public:
	double x1,y1,x2,y2;
	BoundingBox(double xx1, double yy1, double xx2, double yy2) {
		x1 = xx1;
		y1 = yy1;
		x2 = xx2;
		y2 = yy2;
	}
	~BoundingBox(){

	}

};

#endif /* BOUNDINGBOX_H_ */
