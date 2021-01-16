// Test file to be removed

#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include "randomFunc.h"
#include "Canvas.h"

int main(){


	// load a background
	cv::Mat back = cv::imread("../../unit-tests/roi-test/bck.jpg");
	
	// load an ooi
	cv::Mat ooi = cv::imread("../../unit-tests/roi-test/selfie.png");

	// create a randomColor
	vector<int> colors = {randomInt(10, 255), randomInt(10, 255), randomInt(10, 255)};

	vector<int> mod = {4, 5};

	// try to create a canvas  
	Canvas canvas(ooi, back, 6, mod, true, colors);

	vector<vector<int>> rois = canvas.getRois();

	cv::Mat image = canvas.getCanvas();

	for (int i = 0; i < rois.size(); i ++){
	
	
		vector<int> curr = rois.at(i);
		cv::Rect rect(curr.at(0), curr.at(1), curr.at(2), curr.at(3));
		cv::rectangle(image, rect, cv::Scalar(0, 255, 0));
	}

	cv::imshow("canvas",image);
	cv::imshow("mask",canvas.getMask());
	int k = cv::waitKey(0);

	return 0;

}
