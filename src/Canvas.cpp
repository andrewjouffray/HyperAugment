#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>

using namespace std;

class Canvas{

	public:
		int width;
		int height;
		float[5] aspectRatios = {1.33, 1.66, 1.78, 1.85, 2.39};
		cv::Mat canvas;
		cv::Mat mask;
		cv::Mat background;
		cv::Mat ooi;
		vector<vector<int>> rois;
		int maxObjects;
		int numObjects;
		int columnWidth;
		string pathToCanvas;

		// constructor
		Canvas(cv::Mat ooiArg, string canvasPathArg, cv::Mat backgroundArg, int maxOoi){

			ooi = ooiArg;
			pathToCanvas = canvasPathArg;
			background = backgroundArg;
			maxObjects = maxOoi;

			// define the size random between 500 and 720 pixels
			height = rand() % 220 + 500;

			// gets a aspect random ratio from the ratio list
			int randRatio = rand() % 5 + 1;
			randRatio --;
			float ratio = aspectRatios[randRatio];	
			float fWidth = height * ratio;
		      	width = (int)fWidth; // gets a with value as integer

			// create canvas and set it to black canvas to black
			canvas = cv::Mat image(320, 240, CV_8UC3, cv::Scalar(0, 0, 0));
		
		}

		// generates the image
		void createCanvas(){
		
			//TODO:write it
		
		
		}

		void lowerRes(){
			//TODO: write it
		}

		void blurr(){
		
			//TODO:write it
		}

		void tint(){
		
			//TODO:write it
		}

		void ChangeGamma(){
			// write it 
		}

		cv::Mat getCanvas(){
		
			// write it
		}

		cv::Mat getMask(){
		
			//wite it
		}

		vector<vector<int>> getRois(){
		
			//write it
		}





}
