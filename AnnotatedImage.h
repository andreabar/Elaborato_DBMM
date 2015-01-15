/*
 * AnnotatedImage.h
 *
 *  Created on: 13/gen/2015
 *      Author: federico
 */

#ifndef ANNOTATEDIMAGE_H_
#define ANNOTATEDIMAGE_H_

#include <string>
#include <vector>

#include "BoundingBox.h"
using namespace std;

class AnnotatedImage {
public:
	string imgPath;
	vector<BoundingBox> faces;

	AnnotatedImage(string img, std::vector<BoundingBox> annotations) {
		imgPath = img;
		faces = annotations;
	}
	~AnnotatedImage(){

	}
};

#endif /* ANNOTATEDIMAGE_H_ */
