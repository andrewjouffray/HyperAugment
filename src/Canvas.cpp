
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
		vector<vector<int>> rois;
		vector<Ooi> objects; // I might need to define the type Ooi?
		bool debug = false;

		// constructor, params need to be added I think.
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
			height = randomInt(500, 860);

			// gets a aspect random ratio from the ratio list
			int randRatio = randomInt(0, 4);
			float ratio = aspectRatios[randRatio];	
			float fWidth = height * ratio;
		      	width = (int)fWidth; // gets a with value as integer

			// create canvas and set it to black canvas to black
			cv::Mat blkImage(320, 240, CV_8UC3, cv::Scalar(0, 0, 0));
			canvas = blkImage;

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
				Ooi objectOfInterest = Ooi(ooi, columnWidth, height, absolutePos, modProb, debug);
				objects.push_back(objectOfInterest);

				// get the position of the object in the image
				int[4] positions = objectOfInterest.getPosition();
				int x1 = positions[0];
				int y1 = positions[1];
				int x2 = positions[2];
				int y2 = positions[3];

				// overlay the object onto the canvas 
				try{
					cv::Mat objectImage = objectOfInterest.getObject();
					cv::Mat inset(canvas, cv::Rect(x1, y1, x2, y2));
					objectImage.copyTo(inset);
				
				}catch{

					// maybe add a counter for failures?
					if(debug) {
					
						cout << "> (canvas) could not insert object into canvas" << endl;
						cout << "> (canvas) x1, y1, x2, y2: "+x1+", "+y1+", "+x2+", "+y2 << endl;
						cout << "> (canvas) canvas size, rows, cols: " +canvas.rows+", "+canvas.cols;
					}
					else{
						
						cout << "> (canvas) could not insert object into canvas";
					}
				
				}	

				// Creates a mask of the silouette of the object of interest in a color unique 
				// to the label 
				int testColor [3] = {210, 30, 78}; // make sure to properly implement this.
				createMasks(testColor);


				// Calculates an accurate position of all the objects of interest to draw bonding boxes arund them
				rois = calculateRois();
				
				
				// replaces black with a background image
				addBackground();


			
			}
		

		
		
		}

		void lowerRes(){

			// reduction factor
			float factor = 0.0;

			//allows the resolution do be reduced more on larger images
			if (height > 720){
				factor = randomFloat(1.0, 2.0);
			}else{
				factor = randomFloat(1.0, 1.5);
			}

			float newHeightf = height/factor;
			float newWidthf = width/factor;
			int newWidth = (int)newWidthf;
			int newHeight = (int)newHeightf;

			cv::Mat lowRes;
			cv::resize(canvas, lowRes, cv::(newWidth, newHeight));
			cv::resize(lowRes, canvas, cv::(width, height));

		}

		void blurr(){

			int kernelSize = randomInt(1, 5);

			if(kernelSize % 2 == 0){
				kernelSize = kernelSize + 1;
			}
		
			cv::GaussianBlur(canvas, canvas, cv::Size(kernelSize, kernelSize), 0);

		}

		void tint(){
		
			//TODO:write it
		}

		// each label in a dataset has a different colored mask
		void createMasks(int mcolors [3]){

			// FUTURE UPDATE: get the thresholded value once and not in each method

			mask = canvas.clone()
        		cv::cvtColor(mask, mask, cv::COLOR_BGR2GRAY);
        		double thresh = 0;
        		double maxValue = 255;
        		// Binary Threshold
        		cv::threshold(mask, mask, thresh, maxValue, cv::THRESH_BINARY);
        		cv::Mat grayMask;
        		cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR);

			// assing the black mask used in other methods
			blackMask = maks.clone();

        		inRange(mask, cv::Scalar(255, 255, 255), cv::Scalar(255, 255, 255), grayMask);
        		mask.setTo(cv::Scalar(mcolors[0], mcolors[1], mcolors[2]), grayMask);

		}

		vector<vector<int>> calculateRois(){
		
			// write it
		}

		// adds a background image to the black background of the canvas
		cv::Mat addBackground(){
	
        		// resize the background to fit the canvas
        		cv::resize(background, background, cv::Size(blackMask.cols, blackMask.rows));

			// all balck pixels become the bacjgorund image
        		cv::Mat comb = blackMask + background;

			// all remaining white pixels become black
        		comb.setTo(0, comb == 255); // comb means combined images

			// all black pixels are set to canvas
        		cv::Mat comb2 =  comb + canvas;
        		
			canvas = comb2;
	
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
