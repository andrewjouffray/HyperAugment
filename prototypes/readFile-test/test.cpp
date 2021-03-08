#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

//https://stackoverflow.com/questions/701648/create-xml-files-from-c-program

namespace fs = std::filesystem;
using namespace std;

vector<vector<string>> parseFile( )
{

	string line;
	ifstream myfile ("../config.txt");
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

void setSettings (vector<vector<string>> file){

	int affineProb = 50;
	int changeSatProb = 30;
	int changeBrightProb = 30;
	int blurrProb = 30;
	int mult = 30;
	int objects = 40;
	string pathToBackground;
	string output;
	string label;
	string dataset;

	for(vector<string> line : file){
		
		cout << "\n new line: ";
		int i = 0;
		for(string word : line){
		
			if(word.compare("dataset") == 0){
			
				dataset = line[i+1];

			}else if(word.compare("label") == 0){
			
				label = line[i+1];
			}


			i++;
		}

	}



}

int main(int argc, char** argv){

	vector<vector<string>> file = parseFile();


	return 0;
}

