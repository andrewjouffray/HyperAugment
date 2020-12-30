#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include "randomFunc.h"

using namespace std;


// this tests the radom functions, lowe res, blurr, create canvas and inserting an image onto the canvas

cv::Mat replace(cv::Mat image, cv::Mat background){


	// resize the background to fit the canvas
	cv::resize(background, background, cv::Size(image.rows, image.cols));


	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	double thresh = 0;
	double maxValue = 255;
	// Binary Threshold
	cv::threshold(image, image, thresh, maxValue, cv::THRESH_BINARY);
	cv::Mat mask;
	cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);

	for(int i=0; i<image.rows; i++){
    		for(int j=0; j<image.cols; j++){

			std::cout << image.at<uchar>(i,j) << endl;

		
		}
	}


	return image;

}

int main(){

	cv::Mat objectImage = cv::imread("../selfie.png");
	cv::Mat back = cv::imread("../bck.jpg");

	cout << "resising an image" << endl;
	cv::resize(objectImage, objectImage, cv::Size(300, 300));

	cout << "creating an image" << endl;
	imshow("object",objectImage);
	int k = cv::waitKey(0);
	cv::Mat canvas(600, 1200, CV_8UC3, cv::Scalar(0,0,0));

	cout << "putting the object in the image" << endl;
	// define x1, x2, y1, y2
	int x1 = randomInt(30, 300);
	int y1 = randomInt(30, 300);
	int x2 = objectImage.cols;
	int y2 = objectImage.rows;
	cv::Mat insetImage(canvas, cv::Rect(x1, y1, x2, y2));
	objectImage.copyTo(insetImage);

	cout << "applying transformations" << endl;

	canvas = replace(canvas.clone(), back);

	imshow("canvas",canvas);
	k = cv::waitKey(0);
	

	return 0;



}

