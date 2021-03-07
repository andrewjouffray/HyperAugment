#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

//https://stackoverflow.com/questions/701648/create-xml-files-from-c-program

namespace fs = std::filesystem;
using namespace std;

void loadFile( )
{

	string line;
	ifstream myfile ("../config.txt");
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
				if (token.compare(" ") == 0){
				
				}
				splitLine.push_back(token);
    				line.erase(0, pos + delimiter.length());
			}

			splitLine.push_back(line);
			for(string word: splitLine){

				cout << " split= " << word << endl;
			
			}
    		}
   	 myfile.close();
 	
	}
  	else{ cout << "Unable to open file";

	}
}	

int main(int argc, char** argv){

	loadFile();

	return 0;
}

