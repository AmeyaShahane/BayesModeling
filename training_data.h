#pragma once

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <fstream>
#include <string>

using namespace std;

class Model {

private:

	vector<pair<int, int>> LabelCounts; // Number of counts
	vector<pair<int, double>> LabelChances; // Probabilty for labels
	vector<pair<int, vector<int>>> PixelCounts; // Number of pixels
	vector<pair<int, vector<double>>> PixelChances; // Probability for pixels
	int kConfusionMatrix[10][10]; // Initialized confusion matrix n x n 
	double kSmoothingFactor = 0.5; // Smoothing factor
	double kV = 2.0; // KV value 2.0 for binary 

	void SetLabelCounts(vector<pair<int, vector<int>>> training_data); 
	 
	void SetLabelProbabilities(vector<pair<int, vector<int>>> training_data); 

	void SetPixelCounts(vector<pair<int, vector<int>>> training_data); 

	void SetEmptyProbabilities(vector<double> initial); 

	int SetLargestValue(vector<pair<int, double>> prediction); 

	void SetConfusionMatrix(); 

	void PrettyPrint();

public:

	vector<pair<int, vector<double>>> Train();

	int PredictLabel(vector<int> image);

	void ConfusionMatrix();

};