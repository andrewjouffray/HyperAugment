/*
author: Andrew Jouffray
date: feb 2021

The Label is one of outputs of the neural network it's the name of an object that needs to be 
detected by the nework. Each Dataset has one or more labels each containing images of a particual object. Example: The Apple dataset might have labels such as "golden delicious" and "honey crisp" etc... each being a directory containing videos of the apples.

The Label class is instantiated by the Dataset class.

Step1: Find the label directory with the given path
Step2: Find and create the Output directories
Step3: Read the config.yaml and set all the configuration options
Step4: Generate the Canvases (using multithreading)
Step5: Save the Canvases.jpg Masks.jpg and Roi.xml files

sources:

https://stackoverflow.com/questions/18100097/portable-way-to-check-if-directory-exists-windows-linux-c



*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include "randomFunc.h"
#include "Canvas.h"
#include <sys/stat.h>

namespace fs = std::filesystem;
using namespace std;

class Label{

public:

	String inputPath;
	String outputPath;
	String backgroundPath;
	vector<String> backgrounds;
	vector<String> inputs;
        string masks;
        string imgs;
        string xml;

Label(String background, String input, String output){

	inputPath = input;
	outputPath = output;
	backgroundPath = background;

	// three
        string masks = outputPath + "masks/";
        string imgs = outputPath + "imgs/";
        string xml = outputPath + "xml/";

	// gets the list of inputs and background files
	backgrounds = getBackgrounds();
	inputs = getInputs();

	// creates the output directories to save everything in
	bool valid = createOutputDirs();


}

void saveImg(cv::Mat img, string name){

	string path = imgs + name;
	cv::imwrite(path, img);
}

void saveMask(cv::Mat mask, string name){

	string path = masks + name;
	cv::imwrite(path, mask);

}

void saveXML(vector<vector<int>> rois, string name){

	// need to test this

}

// checks and creates all the output directories for a Label
bool createOutputDirs(){

        if (!dirExists(outputPath.c_str())){

                cout << "> Label: " << outputPath << " does not exist." << endl;

                if(fs::create_directory(outputPath)){
                        cout << "> Label: created " << outputPath << endl;
                }else{

                        cout << "> Label: could not create " << outputPath;
                        throw "Error: Could not create output path.";
                }

        }

        if(dirExists(masks.c_str())){
                cout << "> Output for masks/ already exists, using existing directory" << endl;
        }else{

                if(fs::create_directory(masks)){
                        cout << "> Label: created " << masks << endl;
                }else{

                        cout << "> Label: could not create " << masks << endl;
                        throw "Error: Could not create masks path.";
                }

        }

        if(dirExists(imgs.c_str())){
                cout << "> Output for imgs/ already exists, using existing directory" << endl;
        }else{

                if(fs::create_directory(imgs)){
                        cout << "> Label: created " << imgs << endl;
                }else{

                        cout << "> Label: could not create " << imgs << endl;
                        throw "Error: Could not create imgs path.";
                }

        }

        if(dirExists(xml.c_str())){
                cout << "> Output for xml/ already exists, using existing directory" << endl;
        }else{

                if(fs::create_directory(xml)){
                        cout << "> Label: created " << xml << endl;
                }else{

                        cout << "> Label: could not create " << xml << endl;
                        throw "Error: Could not create xml path.";
                }

        }

        return true;



}


// checks if a directory exists
int dirExists(const char* const path)
{
    struct stat info;

    int statRC = stat( path, &info );
    if( statRC != 0 )
    {
        if (errno == ENOENT)  { return 0; } // something along the path does not exist
        if (errno == ENOTDIR) { return 0; } // something in path prefix is not a dir
        return -1;
    }

    return ( info.st_mode & S_IFDIR ) ? 1 : 0;
}

// saves all the names of the files in a given path
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


// returns the list of all .avi or .mp4 files to be run my the augmentation algorithm.
vector<string> getInputs(){

        vector<string> inputs;

        if (dirExists(inputPath.c_str())){
                inputs = getFiles(inputPath);
        }else{
                cout << "> Label: error could not find path " << inputPath << endl;
                throw "> Error: Invalid input path.";
        }

        return inputs;
}

// returns a shuffled list of all the .jpg files to be used as a background.
vector<string> getBackgrounds(){

        vector<string> backgrounds;

        if (dirExists(backgroundPath.c_str())){
                backgrounds = getFiles(backgroundPath);
        }else{
                cout << "> Label: error could not find path " << backgroundPath << endl;
                throw "> Error: Invalid background image path.";
        }

        // shuffle the background images around 
        auto rng = default_random_engine {};
        shuffle(begin(backgrounds), end(backgrounds), rng);

        return backgrounds;


}





}

// load config file specific to it
//
// load all backgrounds
//
// iterate over each video files
//
// for each frame, segment it 
//
// create a x canvases
//
// save them
