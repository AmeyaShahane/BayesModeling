#include "pch.h"
#include "test_label_reader.h"
#include "training_data.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

/* 
Trains the model by getting pixel probabilities at each location and total counts probabilities 
*/ 
vector<pair<int, vector<double>>> Model::Train() {
	LoadData read_files;

	vector<pair<int, vector<int>>> training_data = read_files.TestDataRetriever("C:\\Users\\Ameya\\Downloads\\digitdata\\traininglabels",
		"C:\\Users\\Ameya\\Downloads\\digitdata\\trainingimages");
	
	SetLabelCounts(training_data);
	SetLabelProbabilities(training_data);

	vector<int> initial;
	int i = 0;
	while (i < 784) {
		initial.push_back(0);
		i++;
	}
 
	for (int i = 0; i < 10; i++) {
		PixelCounts.push_back(std::pair<int, vector<int>>(i, initial));
	}

	/*
	Counts how many of each pixel there is
	*/
	SetPixelCounts(training_data);

	vector<double> init;
	int k = 0;
	while (k < 784) {
		init.push_back(0.0);
		k++;
	}

	/*
	Makes empty probability vector 
	*/
	SetEmptyProbabilities(init);

	/*
	Applies soothing factor formula for each pixel
	*/
	for (int i = 0; i < PixelCounts.size(); i++) {
		for (int j = 0; j < 784; j++) {
			PixelChances[i].second[j] = (kSmoothingFactor + PixelCounts[i].second[j]) / (kV * 
				kSmoothingFactor + LabelCounts[i].second);
		}
	}
	return PixelChances;
}

/*
Takes each label and corresponds it with the number of times each label comes.
*/
void Model::SetLabelCounts(vector<pair<int, vector<int>>> training_data) {

	int i = 0;
	while (i < 10) {
		LabelCounts.push_back(std::pair<int, int>(i, 0));
		i++;
	}

	for (int i = 0; i < training_data.size(); i++) {
		LabelCounts[training_data[i].first].second += 1;
	}

}
	
/*
Gives weight of each probability with respect the number of times each label appears.
*/
void Model::SetLabelProbabilities(vector<pair<int, vector<int>>> training_data) {
	
	for (int i = 0; i < 10; i++) {
		LabelChances.push_back(std::pair<int, double>(i, LabelCounts[i].second 
			/ (training_data.size() * 1.0)));
	} 
}

/*
Sets 784 pixels as an image and finds out the probability of each number being given value. 
*/
void Model::SetPixelCounts(vector<pair<int, vector<int>>> training_data) {
	
	for (int i = 0; i < training_data.size(); i++) {
		for (int j = 0; j < 784; j++) {
			if (training_data[i].second[j] == 1) {
				PixelCounts[training_data[i].first].second[j] += 1;
			}
			else {
				continue;
			}
		}
	}
}

/*
Makes empty vector of probabilities.
*/
void Model::SetEmptyProbabilities(vector<double> initial) {

	for (int i = 0; i < 10; i++) {
		PixelChances.push_back(std::pair<int, vector<double>>(i, initial));
	}
}

/*
Gives the likeliness of each label being the handwritten image
*/
int Model::PredictLabel(vector<int> image) {
	
	vector<pair<int, double>> prediction;
	int i = 0;
	while (i < 10) {
		prediction.push_back(pair<int, double>(i, log(LabelChances[i].second)));
		i++;
	}

	for (int i = 0; i < image.size(); i++) {
		if (image[i] == 1) {
			for (int j = 0; j < prediction.size(); j++) {
				prediction[j].second += log(PixelChances[j].second[i]);
			}
			continue;
		}
		for (int j = 0; j < prediction.size(); j++) {
			prediction[j].second += log(1 - PixelChances[j].second[i]);
		}
		
	}

	return SetLargestValue(prediction);
}

/*
Takes highest probability of likeliness of number 
*/
 int Model::SetLargestValue(vector<pair<int, double>> prediction) {
	
	int kBiggestPrediction = prediction[0].second;
	int kLargest = 0;
	for (int i = 0; i < prediction.size(); i++) {
		if (prediction[i].second > kBiggestPrediction) {
			kLargest = prediction[i].first;
			kBiggestPrediction = prediction[i].second;
		}
	}
	return kLargest;
}
 
 /*
 Makes confusion matrix 
 */
void Model::ConfusionMatrix() {
	
	SetConfusionMatrix();

	LoadData FilesReader;
	vector<pair<int, vector<int>>> TestData = FilesReader.TestDataRetriever("C:\\Users\\Ameya\\Downloads\\digitdata\\testlabels",
		"C:\\Users\\Ameya\\Downloads\\digitdata\\testimages");

	for (int i = 0; i < TestData.size(); i++) {
		int kVertCord = PredictLabel(TestData[i].second);

		/*
		Increments the value at the same index of the predicted value in the label's row
		*/
		kConfusionMatrix[kVertCord][PredictLabel(TestData[i].second)] += 1;
	}

	/*
	Prints Confusion Matrix before giving proportion 
	*/
	PrettyPrint();
	
	/*
	Place holders for i and j 
	*/
	int FirstCounter = 0; 
	int SecondCounter = 0;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			FirstCounter += kConfusionMatrix[i][j];
			if (i == j) {
				SecondCounter += kConfusionMatrix[i][j];
			}
		}
	}
	cout << (double)SecondCounter / (double) (FirstCounter);
}

/*
Ensures that confusion matrix is properly printed out 
*/
void Model::PrettyPrint() {
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			std::cout << kConfusionMatrix[i][j] << "    ";
		}
		std::cout << "\n";
	}
}


/* 
Ensures that matrix is properly initialized as 2D matrix array.
*/
void Model::SetConfusionMatrix() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			kConfusionMatrix[i][j] = 0;
		}
	}
}


