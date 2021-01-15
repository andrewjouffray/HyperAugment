#ifndef CANVAS_H
#define CANVAS_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include "Ooi.h"

using namespace std;

class Canvas{

public:
	int width;
	int height;
	int maxObjects;
	int numObjects;
	int columnWidth;
	int[2] modProb; // modification probability
	float[5] aspectRatios = {1.33, 1.66, 1.78, 1.85, 2.39};
	cv::Mat canvas;
	cv::Mat mask;
	cv::Mat blackMask; // bgr black and white mask of the image 
	cv::Mat background;
	cv::Mat ooi;
	vector<cv::rect> rois;
	vector<Ooi> objects; // I might need to define the type Ooi?
	bool debug = false;

	Canvas(cv::Mat ooiArg, cv::Mat backgroundArg, int maxOoi, int modProbability [2], bool debug, int labelColor [3]);

	void createCanvas();

	void lowerRes();

	void blurr();

	void createMasks(int mcolors [3]);

	vector<cv::rect> calculateRois();
			
	cv::Mat addBackground();
	
	void ChangeBrightness();

	cv::Mat getCanvas();
		
	cv::Mat getMask();
		
	vector<vector<int>> getRois();


}

#endif
