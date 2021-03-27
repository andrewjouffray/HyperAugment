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
Step5: Save the Canvases.jpg Masks.jpg and Roi.Label::xml files

sources:

//https://stackoverflow.com/questions/18100097/portable-way-to-check-if-directory-exists-windows-linux-c



*/

Label::Label(string label, string dataset, string output, int affine, int saturation, int bright, int blurr, int lowRes, int canvasQt, int max_obj, vector<string>* input, vector<string>* background, debugArg){

	// pointer to the background array;
	Label::backgrounds = background;

	// pointer to the array of videofiles
	Label::inputs = input;

	Label::labelName = label;
	Label::outputPath = output;
	Label::datasetName = dataset;
	obj_affineProb = affine;
	obj_changeSatProb = saturaton;
	can_changeBrightProb = bright;
	can_blurrProb = blurr;
	can_lowerRes = lowRes;
	canvas_per_frame = canvasQt;
	max_objects = max_obj;

	Label::masks = Label::outputPath + "Label::masks/";
	img = Label::outputPath + "Label::imgs/";
	Label::xml = Label::outputPath + "Label::xml/";
	
	Label::debug= debugArg;
	


	// for each input file
	for(int i = 0; i < Label::inputs->size(); i ++){

		string inFile = Label::inputs->at(i);
		cout << inFile << endl;

		cv::VideoCapture cap(inFile);

        	// create a randomColor for the mask
       		vector<int> colors = {randomInt(10, 255), randomInt(10, 255), randomInt(10, 255)};

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

						cv::Mat background = Label::getRandomBackground();

                                        	// creates and saves a canvas
                                        	Canvas canvas(frame, background, max_objects, obj_affineProb, obj_changeSatProb ,can_changeBrightProb, can_blurrProb, can_lowerRes, Label::debug, &colors);
                                        	int64_t  current = timeSinceEpochMillisec();

						string id = this_thread::get_id();

                                        	string name = to_string(current) + "hpa" + id;

                                        	saveImg(canvas.getCanvas(), name);
						saveMask(canvas.getMask(), name);
						saveXML(canvas.getRois(), name);
                                	}



                        	}

                	} // end of while loop and single region

                // at this point we also wait until all tasks that were created have finished

        	} // end of parallel region

	}



}

void Label::saveImg(cv::Mat img, string name){

	string path = Label::imgs + name + ".jpg";
	cv::imwrite(path, img);
}

void Label::saveMask(cv::Mat mask, string name){

	string path = Label::masks + name + ".jpg";
	cv::imwrite(path, mask);

}

void Label::saveXML(vector<vector<int>> rois, string name, cv::Mat img){

	// image dimentions 
	int height = img.rows;
	int width = img.cols;
	int channels = img.channels();

	string sHeight = to_string(height);
       	string sWidth = to_string(width);
	string sChannel = to_string(channels);

	// save path of the image not the Label::xml
	string fullPath = Label::imgs + name + ".Label::xml";	

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


        TiXmlText * folderName = new TiXmlText( "Label::xml" );
        TiXmlText * fileNameText = new TiXmlText( filename.c_str() );
        TiXmlText * databaseName = new TiXmlText( Label::datasetName.c_str() );
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

                TiXmlText * objectName = new TiXmlText( Label::labelName.c_str() );
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
	string savePath = Label::xml + name + ".Label::xml";
        doc.SaveFile( savePath );
	// need to test this

}

cv::Mat Label::getRandomBackground(){

	int rand = randomInt(0, Label::backgrounds->size());

	string file = Label::backgrounds->at(rand);

	cv::Mat image = cv::imread(file);

	return image;
}




