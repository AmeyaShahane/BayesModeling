#pragma once

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class LoadData {

public:
	vector<pair<int, vector<int>>> TestDataRetriever(string LabelsLocation, string ImagesLocation);
};

