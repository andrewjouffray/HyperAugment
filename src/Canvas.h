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
	vector<int> modProb; // modification probability
	float aspectRatios [5] = {1.33, 1.66, 1.78, 1.85, 2.39};
	cv::Mat canvas;
	cv::Mat mask;
	cv::Mat blackMask; // bgr black and white mask of the image 
	cv::Mat background;
	cv::Mat ooi;
	vector<cv::Rect> rois;
	vector<Ooi> objects; // I might need to define the type Ooi?
	bool debug = false;

	Canvas(cv::Mat ooiArg, cv::Mat backgroundArg, int maxOoi, vector<int> modProbability, bool debug, vector<int> labelColor);

	void createCanvas();

	void lowerRes();

	void blurr();

	void createMasks(vector<int> mcolors);

	vector<cv::Rect> calculateRois();
			
	void addBackground();
	
	void changeBrightness();

	cv::Mat getCanvas();
		
	cv::Mat getMask();
		
	vector<vector<int>> getRois();


};

#endif
