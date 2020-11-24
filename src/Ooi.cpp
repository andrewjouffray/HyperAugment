/*

author: Andrew Jouffray
date: nov 2020

Ooi stands of Objects of interest, those are the objects that are added to the background image (canvas)

The canvas creates between 1 - 10 ooi and needs to place them on the image without overlapping between ooi. So
the algorithm defines a column on the canvas for each of those objects to be passed in. the left boundary of these
columns are the xAbsolutePos. The vertical boundary is the top of the image therefor yAbsolutePos always starts at 0

setp1: rotate
step2: scale
step3: position

note: everytime I mention 'object' in the comments, I'm referring to the object of interest (an image) and not an object in the oop sence.

*/


#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <random>

using namespace std;

class Ooi{

	public 
		int ooiHeight;
		int ooiWidth;
		int xAbsolutePos;
		int yAbsolutePosi;
		int xOffset;
		int yOffset;
		cv::Mat image;
		float maxScale;
		float minScale;
		const int range_from  = 0;
		const int range_to = 1000;
		const int randomValue;
		bool debug = false;

		// constructor
		Ooi(cv::Mat objectOfInterest, int colWidth, int colHeight, int xAbsolutePos, int[2] probabilities){
		
			image = objectOfInterest;

			//define one random value that will be moded, to define probability of transformation
			randomValue = random<int>(range_from, range_to);

                        //Rotate the object to a random angle between 0 and 360
                        int angle = randomValue % 360;
                        rotate(angle);

			// defines the probability that the image will have an affine transform and/or a saturation change
			// 1 = 10%, 2 = 20%, 3 = 33% etc...
			int affineProbability = probabilities[0];
			int saturationProbability = probabilities[1];
			int random = randomValue % 10;
			if (random <= affineProbability){
				affineTransform();
			}
			if (random <= saturationProbability){
				changeSaturation();
			}

			// get the height and width after possible transformations
			ooiWidth = image.size.width();
                        ooiHeight = image.size.height();


			// determine the maximum scale to shrink or expand the image so that it still fits in the canvas
			float maxScaleHeight = (colHeight / ooiHeight) - 0.01;
			float maxScaleWidth = (colWidth / ooiWidth) - 0.01;
			if(maxScaleHeight > maxScaleWidth){
				maxScale = maxScaleWidth;
			}else{
				maxScale = maxScaleHeight
			}
			// I use 1.3 to make sure that the minimum scale smaller than max but still proportional to max, so you
			// don't endup with huge object, and a tiny object the size of three pixel
			minScale = maxScale / 1.3;

			float scale = random(minScale, maxScale);

			// print all scale info when debugging at runtime this can be very helpful
			if (debug){
				cout << "> ===== ooi width and height information ===== " << endl
				cout << "> col height: " + to_string(colHeight) + "| col width: " + to_string(colWidth) << endl;
				cout << "> ooi height: " + to_string(ooiHeight) + "| ooi width: " + to_string(ooiWidth) << endl;
				cout << "> max scale height: " + to_string(maxScaleHeigth) + "| max scale width: " + to_string(maxScaleWidth) << endl;
				cout << "> max scale: " + to_string(maxScale) + "| min scale: " + to_string(minScale) << endl;
			}

			scaleImage(scale);

			//defines the absolute position of the object in the canvas within given boundaries
			try{
				int maxXOffset = colWidth - width;
				int maxYOffset = colHeight - height;
				xOffSet = random<int>(0, maxXOffSet);
            			yOffSet = random<int>(0, maxYOffSet);
			
			}catch(...){
				cout << "> error while computing offsets" << endl;
				xOffSet = 1;
				yOffSet = 1;
			}

			//sets x1 y1
			xAbsolutePos = xOffSet;
			yAbsolutePos = yOffSet;l

			//sets x2 y2
        		ooiWidth += AbsolutePos;
        		ooiHeight += yAbsolutePos;	
		
		}

		// code from user Shoe https://stackoverflow.com/questions/288739/generate-random-numbers-uiniformly-over-an-entire-range
		template<typename T>
		T random(T from, T to) {
    			random_device                    rand_dev;
    			mt19937                          generator(rand_dev());
    			uniform_int_distribution<T>    distr(from, to);
    			return distr(generator);
		}

		// code from user Lars Schillingmann https://stackoverflow.com/questions/22041699/rotate-an-image-without-cropping-in-opencv-in-c
		void rotate(int angle){
			
			cv::Mat src = image;
			// get rotation matrix for rotating the image around its center in pixel coordinates
    			cv::Point2f center((src.cols-1)/2.0, (src.rows-1)/2.0);
    			cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    			// determine bounding rectangle, center not relevant
    			cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
    			// adjust transformation matrix
    			rot.at<float>(0,2) += bbox.width/2.0 - src.cols/2.0;
    			rot.at<float>(1,2) += bbox.height/2.0 - src.rows/2.0;

    			cv::Mat dst;
    			cv::warpAffine(src, dst, rot, bbox.size());

			image = src;
		
		}

		void scaleImage(float scale){
		
			// updates the new height and width and scales the object.	
			ooiWidth = ooiWidth * scale;
			ooiHeight = ooiHeight * scale;
			cv::Size size(ooiWidth, ooiHeight);
			cv::resize(image, image, size);
		}

		void affineTransform(){
		
			//Workig on it here	
		
		}

		void changeSaturation(){	

		
		
		
		}

		int[4] getPosition(){
		
			int[4] positions = {xAbsolutePos, yAbsolutePos, ooiWidth, ooiHeight};
			return positions;
		
		}

		cv::Mat getObject(){
		
			return image;
		
		}





}
