#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include "../include/Dataset.h"

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char** argv){

        Dataset dataset = Dataset("./data/inputs/test_dataset/");

        return 0;
}

