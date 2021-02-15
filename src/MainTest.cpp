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
namespace fs = std::filesystem;


vector<string> getFiles(string path){

        //cout << "adding " + path << endl;
        // this is it for now
        vector<string> files;

        for(const auto & entry : fs::directory_iterator(path)){
                string it = entry.path();

                //cout << it << endl;
                files.push_back(it);
        }

        return files;


}

uint64_t timeSinceEpochMillisec(){
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}




int main(){


	// load a background
	cv::Mat back = cv::imread("../../unit-tests/roi-test/bck.jpg");
	
        string path = "/home/andrew/Projects/Projects/Video/Weeds2/nenuphare/data1595119927.9510028output.avi";
        string save = "/home/andrew/Projects/Projects/augment-test/";
	string saveRoi = save + "rois"

        int64_t  start = timeSinceEpochMillisec();

        cout <<  start << endl;

        cv::VideoCapture cap(path);

	// create a randomColor
	vector<int> colors = {randomInt(10, 255), randomInt(10, 255), randomInt(10, 255)};

	vector<int> mod = {4, 5};

	// create 6 canvases per frame
	int multiply = 6;

	cv::Mat frame;

	#pragma omp parallel    // start the parallel region
        {
                #pragma omp single  // let the while loop execute by one thread and generate tasks
                while (1){

                        if (!cap.read(frame)){

                                cout << "> (Main) No more frames. Terminating." << endl;
                                break;

                        }

                        #pragma omp task
                        {

				for(int i = 0; i < multiply; i ++){
			
					// creates and saves a canvas	
					Canvas canvas(frame, back, 6, mod, false, colors);
                                	int64_t  current = timeSinceEpochMillisec();
                                	string name = save + to_string(current) + "ok.jpg";
                                	cv::imwrite(name, canvas.getCanvas());
				}



                                // this code will be a task that may be executed immediately on a different core or deferred for later execution
                        }

                } // end of while loop and single region

                // at this point we also wait until all tasks that were created have finished

        } // end of parallel region



	// draws the rois, usefull for debugging single threaded
	//for (int i = 0; i < rois.size(); i ++){
	
	
	//	vector<int> curr = rois.at(i);
	//	cv::Rect rect(curr.at(0), curr.at(1), curr.at(2), curr.at(3));
	//	cv::rectangle(image, rect, cv::Scalar(0, 255, 0));
	//}

	//cv::imshow("canvas",image);
	//cv::imshow("mask",canvas.getMask());
	//int k = cv::waitKey(0);
	
	
	uint64_t  end = timeSinceEpochMillisec();

        uint64_t total = end - start;

        cout << end << endl;
        cout << total << endl;


	return 0;

}
