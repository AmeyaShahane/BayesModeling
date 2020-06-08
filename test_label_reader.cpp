#include "pch.h"
#include "test_label_reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

/**
Complements labels and images and reads files into binary format with 1's and 0's using ifstream
**/

// https://stackoverflow.com/questions/1619604/reading-through-file-using-ifstream

vector<pair<int, vector<int>>> LoadData::TestDataRetriever(string LabelsLocation, string ImagesLocation) {

	// vectorizes file contents into binary 1's and 0's 

	std::ifstream infileUseLabel(LabelsLocation);
	std::string LabelLine;
	vector<int> LabelTest;

	while (getline(infileUseLabel, LabelLine)) {
		int label_as_int = atoi(LabelLine.c_str());
		LabelTest.push_back(label_as_int);
	}

	infileUseLabel.close();

	ifstream infile_images(ImagesLocation);
	string kImageLine;
	vector<int> kVectorImage;

	vector<vector <int>> kTestImage;
	string kStringImage = "";

	int kLineLength = 1;

	while (getline(infile_images, kImageLine)) {
		kStringImage += kImageLine;
		if (kLineLength % 28 == 0) {
			int j = 0;
			while (j < kStringImage.size()) {
				if (kStringImage.at(j) == ' ') {
					kVectorImage.push_back(0);
				}
				if (kStringImage.at(j) == '+' || kStringImage.at(j) == '#') {
					kVectorImage.push_back(1);
				}
				if (kStringImage.at(j) == '\n') {
					continue;
				}
				j++;
			}
			kTestImage.push_back(kVectorImage);
			kVectorImage.clear();
			kStringImage = "";
		}
		kLineLength += 1;
	}
	infile_images.close();

	// Pairing label and images
    vector<pair<int, vector<int>>> DataTest;
    for (int i = 0; i < LabelTest.size(); i++) {
		DataTest.push_back(pair<int, vector<int>>(LabelTest[i], kTestImage[i]));
    }
    return DataTest;
}

