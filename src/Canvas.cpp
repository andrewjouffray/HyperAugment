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
		cv::Mat background;
		cv::Mat ooi;
		vector<vector<int>> rois;
		vector<Ooi> objects; // I might need to define the type Ooi?
		bool debug = false;

		// constructor, references to the random functions are passed in
		Canvas(cv::Mat ooiArg, cv::Mat backgroundArg, int maxOoi, int[2] modProbability, bool debug){

			// - check this out when it comes to passing a reference to a function.
			// - https://stackoverflow.com/questions/41959721/passing-function-to-class-in-c
			// - https://stackoverflow.com/questions/7142883/chow-to-pass-reference-to-function-into-another-function
			// - UPDATE: I am leaving the previous comment just in case but I just decided to use a header file and reference the random functions
			// from there
			ooi = ooiArg;
			background = backgroundArg;
			maxObjects = maxOoi;
			modProb = modProbability;

			// define the size random between 500 and 720 pixels
			height = randomInt(500, 720);

			// gets a aspect random ratio from the ratio list
			int randRatio = randomInt(0, 4);
			float ratio = aspectRatios[randRatio];	
			float fWidth = height * ratio;
		      	width = (int)fWidth; // gets a with value as integer

			// create canvas and set it to black canvas to black
			canvas = cv::Mat image(320, 240, CV_8UC3, cv::Scalar(0, 0, 0));

			//define the number of Ooi to be created for this canvas
			numObjects = randomInt(2, maxObjects);

			// one object per column to avoid overlapping of objects on the image
			columnWidth = width / numObjects;

			createCanvas();


		
		}

		// generates the image
		void createCanvas(){

			for(int i = 0; i < numObjects; i ++){

				int absolutePos = columnWidth * i;

				// create an object to put on the canvas
		`		Ooi objectOfInterest = Ooi(ooi, columnWidth, height, absolutePos, modProb, debug);
				objects.push_back(objectOfInterest);

				// get the position of the object in the image
				int[4] positions = objectOfInterest.getPosition();
				int x1 = positions[0];
				int y1 = positions[1];
				int x2 = positions[2];
				int y2 = positions[3];

				// overlay the object onto the canvas 
				try{
					// stuff
				
				}catch{

					// if debug is on
				
				}	

				// createMasks


				// calculateRois
				
				
				// addBackground


			
			}
		

		
		
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

		void createMasks(){
			
			// write it
		}

		void calculateRois(){
		
			// write it
		}

		void addBackground(){
		
			// write it
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
