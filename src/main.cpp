#include <iostream>
#include <fstream>
#include <time.h>

#include <cstring>
#include "neural.h"

//height and width are the same
#define IMG_WID 28
#define IMG_HGT 28
// File to save trained model to
#define TRAIN_FILE "train_data/mnist_train.csv"
#define TEST_FILE "test_data/mnist_test.csv"
#define SAVE_FILE "saved_models/trained_model.txt"

using namespace std;

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

double sigmoidPrime(double x) {
    return (exp(-x) / (pow(1 + exp(-x), 2)));
}

void learningDecay(double& rate) {
    rate *= .995;
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	int num_iter = 0;
    const char* model = NULL;
    cout << "How many iterations would you like to run? (Recommended: 1-10) ";
    cin >> num_iter;

    if(num_iter <= 0) {
        cerr << "Number of iterations is not valid!" << endl;
        exit(1);
    }

    string input;
    do{
        cout << "Do you want to load a trained model (y/n)? ";
        cin >> input;
        if(input == "quit"){
            cout << "Quitting..." << endl;
            exit(0);
        }
    } while(input != "y" && input != "n");

    // Make new model
    if(input == "n") {
        model = "new";
    }
    // Load a trained model
    else if (input == "y") {
        model = "load";
    }


    double learnRate = 0.1;
    bool initType = false;
    // Checks if there is a trained model to load in
    if(!(strcmp(model, "load"))) {
        if (FILE *file = fopen(SAVE_FILE, "r")) {
            fclose(file);
            initType = true;
            // If you pass in a trained model, start with a lower learning rate
            learnRate = 0.05;
        } else {
            cout << "-- No trained model to load, initializing new weights --" << endl;
        }
    }

	string trainFile(TRAIN_FILE);
    string testFile(TEST_FILE);

	int inputNeurons = IMG_WID * IMG_HGT;
	int hiddenNeuron = 20;
	int outputNeurons = 10;

    NeuralNetwork myNetwork = NeuralNetwork(trainFile, testFile, inputNeurons, hiddenNeuron, 
                                            outputNeurons, learnRate, &sigmoid, &sigmoidPrime, initType);
    cout << "--------------------ITERATION 1--------------------" << endl;
    myNetwork.train();

    // loop through multiple iterations, network builds on previous knowledge
    for(int i = 2; i < num_iter + 1; ++i) {
        cout << "--------------------ITERATION " << i << "--------------------" << endl;
        myNetwork.modelInit();
        myNetwork.loadTraining(trainFile);
        learningDecay(learnRate);
        myNetwork.setRate(learnRate);
        myNetwork.train();

    }
    myNetwork.test();
}
