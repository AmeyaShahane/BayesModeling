#include "pch.h"
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm> // for std::copy
#include "training_data.h"
#include "test_label_reader.h"

int main()
{
	
	//std::cout << "Hello World";
	
	Model train_data; 
	train_data.Train(); 
	train_data.ConfusionMatrix(); 

	return 0;
}


