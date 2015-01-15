/*
 * Result.h
 *
 *  Created on: 14/gen/2015
 *      Author: federico
 */

#ifndef RESULT_H_
#define RESULT_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

class Result {
public:
	string imgPath;
	int trueFaces;
	int detectedFaces;
	int correctlyDetectedFaces;
	Result(){

	}
	Result(string img, int t, int d, int c) {
		imgPath = img;
		trueFaces = t;
		detectedFaces = d;
		correctlyDetectedFaces = c;
	}
	~Result(){

	}
};

#endif /* RESULT_H_ */
