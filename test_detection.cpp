

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/all/source.cpp>

#include <string>
#include "json/Value.h"
#include "json/Array.h"
#include <vector>
#include "json/Value.h"
#include "json/Array.h"
#include "json/Object.h"
#include "AnnotatedImage.h"
#include "BoundingBox.h"
#include "Result.h"

using namespace dlib;
using namespace std;

image_window win;
std::vector<Result> results;

void saveResults(){
	Result actualResult;
	JsonBox::Array jsonResults;
	for(int i=0; i<results.size();i++){
		actualResult = results.at(i);
		JsonBox::Object actualImage;
		actualImage["imgPath"] = JsonBox::Value(actualResult.imgPath);
		actualImage["trueFaces"] = JsonBox::Value(actualResult.trueFaces);
		actualImage["detectedFaces"] = JsonBox::Value(actualResult.detectedFaces);
		actualImage["correctlyDetectedFaces"] = JsonBox::Value(actualResult.correctlyDetectedFaces);
		jsonResults.push_back(actualImage);
	}
	JsonBox::Value v(jsonResults);
	v.writeToFile("results");
}


bool isTruePositive(rectangle groundtruth, rectangle detection){
	double intersectionArea = groundtruth.intersect(detection).area();
	double unionArea = (groundtruth + detection).area();
	double alfa = intersectionArea/unionArea;
	if(alfa>0.5){
		return true;
	}else{
		return false;
	}
}

std::vector<AnnotatedImage> extractAnnotations(){
	std::vector<AnnotatedImage> images;
	JsonBox::Value v2;
	v2.loadFromFile("annotations");
	JsonBox::Array names = v2.getArray().at(0).getArray();
	JsonBox::Array allFaces = v2.getArray().at(1).getArray();
	string imgName;
	JsonBox::Array imgFaces;
	JsonBox::Array face;
	JsonBox::Array x1y1;
	JsonBox::Array x2y2;
	double x1,y1,x2,y2;
	BoundingBox* box;
	for (int i=0; i< names.size(); i++){
		imgName = names.at(i).getString();
		imgFaces = allFaces.at(i).getArray();
		std::vector<BoundingBox> annotations;
		if(imgFaces.at(0).getArray().at(0).isArray()){
			for (int j=0; j< imgFaces.size(); j++){
				x1y1 = imgFaces.at(j).getArray().at(0).getArray();
				x2y2 = imgFaces.at(j).getArray().at(1).getArray();
				if(x1y1.at(0).isDouble()){
					x1 = x1y1.at(0).getDouble();
				}else{
					x1 = x1y1.at(0).getInt();
				}
			    if(x1y1.at(1).isDouble()){
			    	y1 = x1y1.at(1).getDouble();
			    }else{
			    	y1 = x1y1.at(1).getInt();
			    }
			    if(x2y2.at(0).isDouble()){
			    	x2 = x2y2.at(0).getDouble();
			    }else{
			    	x2 = x2y2.at(0).getInt();
			    }
			    if(x2y2.at(1).isDouble()){
			    	y2 = x2y2.at(1).getDouble();
			    }else{
			    	y2 = x2y2.at(1).getInt();
			    }
				box = new BoundingBox(x1*2,y1*2,x2*2,y2*2);
				annotations.push_back(*box);
			}
		}else{
			x1y1 = imgFaces.at(0).getArray();
			x2y2 = imgFaces.at(1).getArray();
			if(x1y1.at(0).isDouble()){
				x1 = x1y1.at(0).getDouble();
			}else{
				x1 = x1y1.at(0).getInt();
			}
		    if(x1y1.at(1).isDouble()){
		    	y1 = x1y1.at(1).getDouble();
		    }else{
		    	y1 = x1y1.at(1).getInt();
		    }
		    if(x2y2.at(0).isDouble()){
		    	x2 = x2y2.at(0).getDouble();
		    }else{
		    	x2 = x2y2.at(0).getInt();
		    }
		    if(x2y2.at(1).isDouble()){
		    	y2 = x2y2.at(1).getDouble();
		    }else{
		    	y2 = x2y2.at(1).getInt();
		    }
			box = new BoundingBox(x1*2,y1*2,x2*2,y2*2);
			annotations.push_back(*box);
		}
		AnnotatedImage* annotated = new AnnotatedImage(imgName, annotations);
		images.push_back(*annotated);
	}
	return images;
}


void face_detection(string image, std::vector<BoundingBox> boundingBoxes)
{
    try
    {
        frontal_face_detector detector = get_frontal_face_detector();
        cout << "processing image " << image << endl;
        array2d<unsigned char> img;
        load_image(img, image);
        pyramid_up(img);
        win.clear_overlay();
        win.set_image(img);
        std::vector<rectangle> groundtruths;
        for(int i=0; i<boundingBoxes.size(); i++){
        	BoundingBox face = boundingBoxes.at(i);
        	groundtruths.push_back(*new rectangle(face.x1, face.y1, face.x2, face.y2));
        }
        cout << "Number of faces: " << boundingBoxes.size() << endl;
        std::vector<rectangle> dets = detector(img);
        cout << "Number of faces detected: " << dets.size() << endl;
        win.add_overlay(dets, rgb_pixel(255,0,0));
    	win.add_overlay(groundtruths, rgb_pixel(0,0,255));
    	int faceCorrectlyDetected = 0;
        for(int i=0; i<dets.size();i++){
        	rectangle actualDet = dets.at(i);
        	bool found = false;
        	int j=0;
        	while(!found && j<groundtruths.size()){
        		rectangle actualGroundtruth = groundtruths.at(j);
        		if(isTruePositive(actualGroundtruth,actualDet)){
        			faceCorrectlyDetected++;
        			found=true;
        		}
        		j++;
        	}
        }
        results.push_back(*new Result(image, groundtruths.size(),dets.size(),faceCorrectlyDetected));
        stringstream n;
        n << faceCorrectlyDetected;
        cout<< "Face correctly detected: " + n.str() << endl;
    }
    catch (exception& e)
    {
        cout << "\n Exception thrown! " << endl;
        cout << e.what() << endl;
    }
}


int main(int argc, char** argv)
{
	std::vector<AnnotatedImage> annotatedImages = extractAnnotations();
    try
    {
    	string image;
    	std::vector<BoundingBox> boundingBoxes;
    	for(int i=0; i<annotatedImages.size();i++){
    		image = annotatedImages.at(i).imgPath;
    		boundingBoxes = annotatedImages.at(i).faces;
    		face_detection("testimages/" + image, boundingBoxes);
            cout << "Hit enter to process the next image..." << endl;
            cin.get();
    	}
    	saveResults();
        return 0;
    }
    catch (exception& e)
    {
    	cout << "\nexception thrown!" << endl;
    	cout << e.what() << endl;
    }
}



