#ifndef OOI_H
#define OOI_H


#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <random>
#include "randomFunc.h"

using namespace std;

class Ooi{

public 
	int ooiHeight;
	int ooiWidth;
	int xAbsolutePos;
	int yAbsolutePosi;
	int xOffset;
	int yOffset;
	const int randomValue;
	float maxScale;
	float minScale;
	bool debug = false;
	cv::Mat image;

	Ooi(cv::Mat objectOfInterest, int colWidth, int colHeight, int xAbsolutePos, int[2] probabilities, bool debug);

	void rotate(int angle);

	void scaleImage(float scale);

	void affineTransform();

	void changeSaturation();	

	int[4] getPosition();

	cv::Mat getObject();
		


}

#endif
