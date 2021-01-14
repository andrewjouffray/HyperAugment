/*
 
author: Andrew Jouffray
date: jan 2021

A Canvas is the image that the neural network will end up seeing, millions of unique canvases can be generated if needed. Each canvas is a backround image, 
and between 2 and 10 object of interest copied on top of the background image, each canvas object has a mask where a detailed outline of the objects in interest in the image 
are available for the model to learn, Bounding boxes are also available to the model.

step1: define the size and aspect ratio of the canvas 
step2: create the Ooi and copy them on top of the canvas
step3: create a mask of the canvas
step4: get the Bounding box coordinates of each object in the canvas
step5: replace the bacl pixels with the provided backgorund image
step6: randomely apply transforamtions: (resolution change, blurr, brightness change)

*/
#include "Canvas.h"


// constructor, params need to be added I think.
Canvas::Canvas(cv::Mat ooiArg, cv::Mat backgroundArg, int maxOoi, int[2] modProbability, bool debug){

	ooi = ooiArg;
	background = backgroundArg;
	maxObjects = maxOoi

	// probablity of modifying the Oois not the whole canvas			
	modProb = modProbability;

	// used to randomely apply transformations to the canvas
	canvasModProb = randomInt(1, 100);

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

	// Creates a mask of the silouette of the object of interest in a color unique 
	// to the label 
	int testColor [3] = {210, 30, 78}; // make sure to properly implement this.
	createMasks(testColor);


	// Calculates an accurate position of all the objects of interest to draw bonding boxes arund them
	rois = calculateRois();
				
				
	// replaces black with a background image
	addBackground();

	// transformations
	if (canvasModProb % 2 == 0){
		changeBrightness();
	}

	if (canvasModProd > 75){
		blurr();
	}

	if (canvasModProd % 5 == 0){
		lowerRes();
	}


		
}

// generates the image
void Canvas::createCanvas(){

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

			
	}
		

		
		
}

void Canvas::lowerRes(){

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

void Canvas::blurr(){

	int kernelSize = randomInt(1, 5);

	if(kernelSize % 2 == 0){
		kernelSize = kernelSize + 1;
	}
		
	cv::GaussianBlur(canvas, canvas, cv::Size(kernelSize, kernelSize), 0);

}

// each label in a dataset has a different colored mask
void Canvas::createMasks(int mcolors [3]){

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

vector<cv::rect> Canvas::calculateRois(){
			
	// if this breaks it might be due to black mask being in brg and not in gray
				
	int thresh = 100;
        cv::Mat canny_output;
        cv::Canny(blackMask, canny_output, thresh, thresh*2);

        vector<vector<cv::Point>> contours;
        cv::findContours(canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	vector<vector<cv::Point> > contours_poly( contours.size() );
        vector<cv::Rect> boundRect( contours.size() );

        for( size_t i = 0; i < contours.size(); i++ )
        {
                cout << "iterating over the countours" << endl;

                int areax = cv::contourArea(contours[i]);

                cout << "Area: " << areax << endl;

                // 3000 is a dummy value, best to calculate average
               	if (areax > 3000){
                        cv::approxPolyDP( contours[i], contours_poly[i], 3, true );
                        boundRect[i] = cv::boundingRect( contours_poly[i] );
               	}
	}

	// each rectangle is define like this [x,y,width,height]
	// Point point0 = Point(rectangle.x, rectangle.y);
	return boundRect;


}

// adds a background image to the black background of the canvas
cv::Mat Canvas::addBackground(){
	
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

void Canvas::ChangeBrightness(){

	// HSV stands for Hue Saturation Value(Brightness)
       	cv::cvtColor(Ooi::canvas,Ooi::canvas,CV_BGR2HSV);

       	// hsv values are on a scale from 0 to 255
        int defaultVal = 255;

        int randValue = randomInt(155, 355);

        // value is brightness, we modify this on the whole canvas not on one object, we don't modify it here.
        int value = randValue - defaultVal;

        // iterate over the Ooi::image
        for(int y=0; y<Ooi::image.cols; y++)
        {
                for(int x=0; x<Ooi::image.rows; x++)
                {
                        // get the current values of a pixel
                        int cur3 = Ooi::image.at<Vec3b>(cv::Point(y,x))[2];

                        // add the modified values 
                       	cur3 += value;

                        // wrap the value around if they exceed 255
                        if(cur3 < 0) cur3= 0; else if(cur3 > 255) cur3 = 255;

                        // set the new values 
                        Ooi::image.at<Vec3b>(cv::Point(y,x))[2] = cur3;
                }
        }

        cv::cvtColor(canvas,canvas,cv::CV_HSV2BGR);


}

cv::Mat Canvas::getCanvas(){
		
	return canvas;
}

cv::Mat Canvas::getMask(){
		
	return mask;
}

vector<vector<int>> Canvas::getRois(){

	vector<vector<int>> allRois;

	for (auto & rectangle : rois) {

		// possible error area make sure this is the propper order
		// make sure they are int
		// make sure width and height are the actuall references
		vector<int> currRect;
		currRect.push_back(rectangle.x);
		currRect.push_back(rectangle.y);
    		currRect.push_back(rectangle.width);
		currRect.push_back(rectangle.height);

		allRois.push_back(currRect);
	}		

	return allRois;
			
}





}
