
#include "../include/Dataset.h"

Dataset::Dataset(string pathToYeet){ // load the config from yeet file

	// reads the config file and sets all the parameters
	vector<vector<string>> file = parseFile(pathToYeet);
	setSettings(file);

	// gets the list of inputs and background files
	Dataset::backgrounds = getBackgrounds();

	// gets all the Dataset::labels
	Dataset::labels = getLabels();

	// creates the output directories to save everything in
	bool valid = createOutputDirs();

	for (string label : Dataset::labels){
	

		cout << "============== creating a mock Label onject =========" << endl;
		cout << "Label " << label << endl;
		cout << "Dataset " << Dataset::datasetName << endl;
		cout << "Input path " << Dataset::inputPath << endl;
		cout << "Output path " << Dataset::outputPath << endl;
		cout << "Object affine %" << Dataset::obj_affineProb << endl;
		cout << "Dataset::obj_changeSatProb " << Dataset::obj_changeSatProb << endl;
		cout << "Dataset::can_changeBrightProb " << Dataset::can_changeBrightProb << endl;
		cout << "Dataset::can_blurrProb " << Dataset::can_blurrProb << endl;
		cout << "Dataset::can_lowerRes " << Dataset::can_lowerRes << endl;
		cout << "Dataset::canvas_per_frame " << Dataset::canvas_per_frame << endl;
		cout << "Dataset::max_objects " << Dataset::max_objects << endl;
		cout << "===== mock video files to be augmented ======" << endl;
		vector<string> videoFiles = getFiles(Dataset::inputPath);
		for(string video : videoFiles){
		
			cout << video << endl;
		}

	}



}

// checks and creates all the output directories for a Label
bool Dataset::createOutputDirs(){

	cout << "Creating the output directories..." << endl;

        if (!dirExists(Dataset::outputPath.c_str())){

                cout << "> Label: " << Dataset::outputPath << " does not exist." << endl;

                if(fs::create_directory(Dataset::outputPath)){
                        cout << "> Label: created " << Dataset::outputPath << endl;
                }else{

                        cout << "> Label: could not create " << Dataset::outputPath;
                        throw "Error: Could not create output path.";
                }

        }

        if(dirExists(Dataset::masks.c_str())){
                cout << "> Output for Dataset::masks/ already exists, using existing directory" << endl;
        }else{

                if(fs::create_directory(Dataset::masks)){
                        cout << "> Label: created " << Dataset::masks << endl;
                }else{

                        cout << "> Label: could not create " << Dataset::masks << endl;
                        throw "Error: Could not create Dataset::masks path.";
                }

        }

        if(dirExists(Dataset::imgs.c_str())){
                cout << "> Output for Dataset::imgs/ already exists, using existing directory" << endl;
        }else{

                if(fs::create_directory(Dataset::imgs)){
                        cout << "> Label: created " << Dataset::imgs << endl;
                }else{

                        cout << "> Label: could not create " << Dataset::imgs << endl;
                        throw "Error: Could not create Dataset::imgs path.";
                }

        }

        if(dirExists(Dataset::xml.c_str())){
                cout << "> Output for Dataset::xml/ already exists, using existing directory" << endl;
        }else{

                if(fs::create_directory(Dataset::xml)){
                        cout << "> Label: created " << Dataset::xml << endl;
                }else{

                        cout << "> Label: could not create " << Dataset::xml << endl;
                        throw "Error: Could not create Dataset::xml path.";
                }

        }

        return true;



}

// checks if a directory exists
int Dataset::dirExists(const char* const path)
{

    cout << "checking if " << path << " exists..." << endl;
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
vector<string> Dataset::getFiles(string path){

        cout << "adding " + path << endl;
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
vector<string> Dataset::getLabels(){

	cout << "Getting the Labels " << endl;

        vector<string> labels;

	vector<string> files;

        if (dirExists(Dataset::inputPath.c_str())){
                files = getFiles(Dataset::inputPath);
        }else{
                cout << "> Label: error could not find label path " << Dataset::inputPath << endl;
                throw "> Error: Invalid input path.";
        }

	for (string path : files){
	
	
		if (path.length() > 5 && path.substr(path.length() - 4).compare("yeet")){
		
			cout << "Dataset: Ignoring the yeet file" << endl;
		}else{
		
			labels.push_back(path);
			cout << "Dataset: Added label " << path << endl;
		
		}
	}

        return labels;
}

// returns a shuffled list of all the .jpg files to be used as a background.
vector<string> Dataset::getBackgrounds(){

        vector<string> backgrounds;

	cout << "Loading the background from " << Dataset::backgroundPath << endl;

        if (dirExists(Dataset::backgroundPath.c_str())){
                backgrounds = getFiles(Dataset::backgroundPath);
        }else{
                cout << "> Label: error could not find background path " << Dataset::backgroundPath << endl;
                throw "> Error: Invalid background image path.";
        }

        // shuffle the background images around 
        auto rng = default_random_engine {};
        shuffle(begin(backgrounds), end(Dataset::backgrounds), rng);

        return backgrounds;


}

// parses the .yeet file into a vector of string vectors
vector<vector<string>> Dataset::parseFile(string path)
{

	string fileName = "dataset_config.yeet";
	string pathToYeet = path + fileName;
	cout << "parsing file " << pathToYeet << endl;
        string line;
        ifstream myfile (pathToYeet);
        vector<vector<string>> parsedFile;
        if (myfile.is_open()){
                while ( getline (myfile,line) ){
                        //cout << line << '\n';

                        std::string delimiter = "=";

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
void Dataset::setSettings (vector<vector<string>> file){

        for(vector<string> line : file){

                // get the first word of the line
                string word = line.at(0);

                if(word.compare("dataset_name") == 0){

                        Dataset::datasetName = line.at(1);

                }
                else if(word.compare("output_path") == 0){

                        Dataset::outputPath = line.at(1);
                }
                else if(word.compare("background_path") == 0){

                        Dataset::backgroundPath = line.at(1);
                }
                else if(word.compare("max_objects_per_canvas") == 0){

			Dataset::max_objects = stoi(line.at(1));
                }
                else if(word.compare("canvases_per_frame") == 0){

			Dataset::canvas_per_frame = stoi(line.at(1));
                }
                else if(word.compare("canvas_blurr") == 0){

                        Dataset::can_blurrProb = stoi(line.at(1));
                }
                else if(word.compare("object_saturation") == 0){

                        Dataset::obj_changeSatProb = stoi(line.at(1));
                }
                else if(word.compare("canvas_lower_resolution") == 0){

                        Dataset::can_lowerRes = stoi(line.at(1));
                }
                else if(word.compare("canvas_change_brightness") == 0){

                        Dataset::can_changeBrightProb = stoi(line.at(1));
                }
                else if(word.compare("object_affine_transform") == 0){

                        Dataset::obj_affineProb = stoi(line.at(1));
                }
                else if(word.compare("//") == 0){

                        //do nothing
                }


        }
	
	if (Dataset::outputPath.compare("default") == 0){

		string add_path = "/data/output/";

		string current_path = fs::current_path();
	
		Dataset::outputPath = current_path + add_path + Dataset::datasetName + "/";
	}

	// three folders to be created
        Dataset::masks = Dataset::outputPath + "masks/";
        Dataset::imgs = Dataset::outputPath + "imgs/";
        Dataset::xml = Dataset::outputPath + "xml/";

	// prints out all the settings allowing the user to check that everything is ok
        cout << "\n========================= Dataset Configuration ==================================" << endl;
        cout << "> readFile: path to background:                             " << Dataset::backgroundPath << endl;
        cout << "> readFile: output path:                                    " << Dataset::outputPath << endl;
        cout << "> readFile: dataset name:                                   " << Dataset::datasetName  << endl;
        cout << "> readFile: number of canvases created per video frame:     " << Dataset::canvas_per_frame << endl;
        cout << "> readFile: max number of objects to be put in each canvas: " << Dataset::max_objects << endl;
        cout << "\n========================= Canvas Modification ===================================" << endl;
        cout << "> readFile: chances of blurring canvas:                     " << Dataset::can_blurrProb << "%" << endl;
        cout << "> readFile: chances of lowering the canvas resolution:      " << Dataset::can_lowerRes << "%" << endl;
        cout << "> readFile: chances of changing the canvas brightness:      " << Dataset::can_changeBrightProb << "%" << endl;
        cout << "\n========================= Object Modification ===================================" << endl;
        cout << "> readFile: chances of changing object color saturation:    " << Dataset::obj_changeSatProb << "%" << endl;
        cout << "> readFile: chances of changing object affine transform:    " << Dataset::obj_affineProb << "%" << endl;

}



