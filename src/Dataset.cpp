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

//https://stackoverflow.com/questions/18100097/portable-way-to-check-if-directory-exists-windows-linux-c



*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include "./inculde/andomFunc.h"
#include "./include/Canvas.h"
#include <sys/stat.h>
#include "./vendors/tinyxml/tinyxml.h"
#include "./vendors/tinyxml/tinystr.h"


namespace fs = std::filesystem;
using namespace std;

class Dataset{

public:

Dataset(){ // load the config from yeet file

	// read it's config file 
	
	// interate over the sub_dir
	
	// read their config file 
	
	// check all values in the config file
	
	// check all paths
	
	// good error handling 

	// reads the config file and sets all the parameters
	vector<vector<string>> file = parseFile(pathToYeet);
	setSettings(file);

	// gets the list of inputs and background files
	backgrounds = getBackgrounds();
	inputs = getInputs();

	// creates the output directories to save everything in
	bool valid = createOutputDirs();

	// for each input file
	for(string inFile : inputs){

		// make sue the input is a full path not just 
		cout << inFile << endl;

		cv::VideoCapture cap(inFile);

        	// create a randomColor for the mask
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

                                	cout << "> (Label) No more frames." << endl;
                                	break;

                        	}

                        	#pragma omp task
                        	{

                                	for(int i = 0; i < multiply; i ++){

                                        	// creates and saves a canvas
                                        	Canvas canvas(frame, back, 6, mod, false, colors); // <---------------- Update this
                                        	int64_t  current = timeSinceEpochMillisec();
                                        	string name = to_string(current) + "hpa";
                                        	saveImg(canvas.getCanvas(), name);
						saveMask(canvas.getMask(), name);
						saveXML(canvas.getRois(), name, );
                                	}



                        	}

                	} // end of while loop and single region

                // at this point we also wait until all tasks that were created have finished

        	} // end of parallel region

	}



}

void saveImg(cv::Mat img, string name){

	string path = imgs + name + ".jpg";
	cv::imwrite(path, img);
}

void saveMask(cv::Mat mask, string name){

	string path = masks + name + ".jpg";
	cv::imwrite(path, mask);

}

void saveXML(vector<vector<int>> rois, string name, string datasetName, string labelName, cv::Mat img){

	// image dimentions 
	int height = img.rows;
	int width = img.cols;
	int channels = img.channels();

	string sHeight = to_string(height);
       	string sWidth = to_string(width);
	string sChannel = to_string(channels);

	// save path
	string fullPath = imgs + name + ".xml";	

	// image being refered to path
	string filename = name + ".jpg";

	TiXmlDocument doc;
        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
        TiXmlElement * annotation = new TiXmlElement( "annotation" );
        TiXmlElement * folder = new TiXmlElement( "folder" );
        TiXmlElement * filename = new TiXmlElement( "filename" );
        TiXmlElement * path = new TiXmlElement( "path" );
        TiXmlElement * source = new TiXmlElement( "source" );
        TiXmlElement * database = new TiXmlElement( "database" );
        TiXmlElement * size = new TiXmlElement( "size" );
        TiXmlElement * width = new TiXmlElement( "width" );
        TiXmlElement * height = new TiXmlElement( "height" );
        TiXmlElement * depth = new TiXmlElement( "depth" );
        TiXmlElement * segmented = new TiXmlElement( "segmented" );


        TiXmlText * folderName = new TiXmlText( "xml" );
        TiXmlText * fileNameText = new TiXmlText( filename.c_str() );
        TiXmlText * databaseName = new TiXmlText( datasetName.c_str() );
        TiXmlText * pathText = new TiXmlText( fullPath.c_str() );
        TiXmlText * widthVal = new TiXmlText( sWidth.c_str() );
        TiXmlText * heightVal = new TiXmlText( sHeight.c_str() );
        TiXmlText * depthVal = new TiXmlText( sChannel.c_str() );
        TiXmlText * segmentedVal = new TiXmlText( "0" );


        annotation->LinkEndChild( folder );
                folder->LinkEndChild(folderName);

        annotation->LinkEndChild( filename );
                filename->LinkEndChild(fileNameText);

        annotation->LinkEndChild( path );
                path->LinkEndChild(pathText);

        annotation->LinkEndChild( source );
                source->LinkEndChild( database );
                        database->LinkEndChild( databaseName );

        annotation->LinkEndChild( size );
                size->LinkEndChild( width );
                        width->LinkEndChild( widthVal );
                size->LinkEndChild( height );
                        height->LinkEndChild( heightVal );
                size->LinkEndChild( depth );
                        depth->LinkEndChild( depthVal );

        annotation->LinkEndChild( segmented );
                segmented->LinkEndChild( segmentedVal );

        // add all the objects here might need to create them in the loop
        for(vector<int> roi : rois){

                TiXmlElement * object = new TiXmlElement( "object" );
                TiXmlElement * name = new TiXmlElement( "name" );
                TiXmlElement * pose = new TiXmlElement( "pose" );
                TiXmlElement * truncated = new TiXmlElement( "truncated" );
                TiXmlElement * difficult = new TiXmlElement( "difficult" );
                TiXmlElement * bndbox = new TiXmlElement( "bndbox" );
                TiXmlElement * xmin = new TiXmlElement( "xmin" );
                TiXmlElement * ymin = new TiXmlElement( "ymin" );
                TiXmlElement * xmax = new TiXmlElement( "xmax" );
                TiXmlElement * ymax = new TiXmlElement( "ymax" );

                TiXmlText * objectName = new TiXmlText( name.c_str() );
                TiXmlText * objectPose = new TiXmlText( "Unspecified" );
                TiXmlText * objectTruncated = new TiXmlText( "0" );
                TiXmlText * objectDifficult = new TiXmlText( "0" );

                string x1 = to_string(roi[0]);
                string y1 = to_string(roi[1]);
                string x2 = to_string(roi[2]);
                string y2 = to_string(roi[3]);

                TiXmlText * objectXmin = new TiXmlText( x1.c_str() );
                TiXmlText * objectYmin = new TiXmlText( y1.c_str() );
                TiXmlText * objectXmax = new TiXmlText( x2.c_str() );
                TiXmlText * objectYmax = new TiXmlText( y2.c_str() );



                annotation->LinkEndChild( object );
                        object->LinkEndChild(name);
                                name->LinkEndChild(objectName);
                        object->LinkEndChild(pose);
                                pose->LinkEndChild(objectPose);
                        object->LinkEndChild(truncated);
                                truncated->LinkEndChild(objectTruncated);
                        object->LinkEndChild(difficult);
                                difficult->LinkEndChild(objectDifficult);
                        object->LinkEndChild(bndbox);
                                bndbox->LinkEndChild(xmin);
                                        xmin->LinkEndChild(objectXmin);
                                bndbox->LinkEndChild(ymin);
                                        ymin->LinkEndChild(objectYmin);
                                bndbox->LinkEndChild(xmax);
                                        xmax->LinkEndChild(objectXmax);
                                bndbox->LinkEndChild(ymax);
                                        ymax->LinkEndChild(objectYmax);


        }





        doc.LinkEndChild( decl );
        doc.LinkEndChild( annotation );
        doc.SaveFile( "labelImgDemo.xml" );
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

// parses the .yeet file into a vector of string vectors
vector<vector<string>> parseFile(string pathToYeet)
{

        string line;
        ifstream myfile (pathToYeet);
        vector<vector<string>> parsedFile;
        if (myfile.is_open()){
                while ( getline (myfile,line) ){
                        //cout << line << '\n';

                        std::string delimiter = " ";

                        vector<string> splitLine;
                        size_t pos = 0;
                        std::string token;
                        while ((pos = line.find(delimiter)) != std::string::npos) {
                                token = line.substr(0, pos);
                                // check if empty char
                                if (token.compare(" ") != 0 && token.compare("") != 0){
                                        splitLine.push_back(token);
                                }
                                line.erase(0, pos + delimiter.length());
                        }
                        if (token.compare(" ") != 0){
                                splitLine.push_back(line);
                        }
                        parsedFile.push_back(splitLine);
                }
         myfile.close();
 
        }
        else{ cout << "Unable to open file";

        }

        return parsedFile;
}

// goes through each line and reads the config
void setSettings (vector<vector<string>> file){

        for(vector<string> line : file){

                // get the first word of the line
                string word = line.at(0);

                if(word.compare("dataset_name") == 0){

                        datasetName = line.at(2);

                }else if(word.compare("label_name") == 0){

                        labelName = line.at(2);
                }
                else if(word.compare("output_path") == 0){

                        outputPath = line.at(2);
                }
                else if(word.compare("background_path") == 0){

                        backgroundPath = line.at(2);
                }
                else if(word.compare("max_objects_per_canvas") == 0){

                        objects = stoi(line.at(2));
                }
                else if(word.compare("canvases_per_frame") == 0){

                        mult = stoi(line.at(2));
                }
                else if(word.compare("canvas_blurr") == 0){

                        can_blurrProb = stoi(line.at(2));
                }
                else if(word.compare("object_saturation") == 0){

                        obj_changeSatProb = stoi(line.at(2));
                }
                else if(word.compare("canvas_lower_resolution") == 0){

                        can_lowerRes = stoi(line.at(2));
                }
                else if(word.compare("canvas_change_brightness") == 0){

                        can_changeBrightProb = stoi(line.at(2));
                }
                else if(word.compare("object_affine_transform") == 0){

                        obj_affineProb = stoi(line.at(2));
                }
                else if(word.compare("//") == 0){

                        //do nothing
                }


        }
	
	if (outputPath.compare("default")){
	
		outputPath = fs::current_path() + "/data/output/" + datasetName + "/";
	}

	// three folders to be created
        masks = outputPath + "masks/";
        imgs = outputPath + "imgs/";
        xml = outputPath + "xml/";

	// prints out all the settings allowing the user to check that everything is ok
        cout << "\n========================= Label Configuration ===================================" << endl;
        cout << "> readFile: path to background:                             " << pathToBackground << endl;
        cout << "> readFile: output path:                                    " << output << endl;
        cout << "> readFile: dataset name:                                   " << dataset << endl;
        cout << "> readFile: label name:                                     " << label << endl;
        cout << "> readFile: number of canvases created per video frame:     " << mult << endl;
        cout << "> readFile: max number of objects to be put in each canvas: " << objects << endl;
        cout << "\n========================= Canvas Modification ===================================" << endl;
        cout << "> readFile: chances of blurring canvas:                     " << can_blurrProb << "%" << endl;
        cout << "> readFile: chances of lowering the canvas resolution:      " << can_lowerRes << "%" << endl;
        cout << "> readFile: chances of changing the canvas brightness:      " << can_changeBrightProb << "%" << endl;
        cout << "\n========================= Object Modification ===================================" << endl;
        cout << "> readFile: chances of changing object color saturation:    " << obj_changeSatProb << "%" << endl;
        cout << "> readFile: chances of changing object affine transform:    " << obj_affineProb << "%" << endl;

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
