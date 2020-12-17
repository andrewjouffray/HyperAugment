#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>

using namespace std;

// defines the types of the random functions that will be passed in
typedef function<float(float,float)> randFloatType;
typedef function<int(int,int)> randIntType;

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
		randIntType randInt;
		randFloatType randFloat;

		// constructor, references to the random functions are passed in
		Canvas(cv::Mat ooiArg, cv::Mat backgroundArg, int maxOoi, const randFloatType & randF, const randIntType & randI){

			// check this out when it comes to passing a reference to a function.
			// https://stackoverflow.com/questions/41959721/passing-function-to-class-in-c
			// https://stackoverflow.com/questions/7142883/chow-to-pass-reference-to-function-into-another-function
			// set the references to the rand function
			randInt = randI;
			randFloat = randF;

			ooi = ooiArg;
			pathToCanvas = canvasPathArg;
			background = backgroundArg;
			maxObjects = maxOoi;

			// define the size random between 500 and 720 pixels
			height = randInt(500, 720);

			// gets a aspect random ratio from the ratio list
			int randRatio = randInt(0, 4);
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
