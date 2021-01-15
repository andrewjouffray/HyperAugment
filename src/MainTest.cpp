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
	cv::Mat back = cv::imread("../unit-tests/roi-test/bck.jpg");
	
	// load an ooi
	cv::Mat ooi = cv::imread("../unit-tests/roi-test/selfie.png");

	// create a randomColor
	int colors [3] = {randomInt(10, 255), randomInt(10, 255), randomInt(10, 255)};

	int mod [2] = {4, 5};

	// try to create a canvas  
	Canvas canvas(ooi, back, 6, mod, true, colors);

	return 0;

}
