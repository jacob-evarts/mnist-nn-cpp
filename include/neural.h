#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <cstring>

#include "../include/matrix.h"

using namespace std;

class NeuralNetwork {
	Matrix X, Y, H1, H2;
	Matrix W1, W2, W3;
	Matrix B1, B2, B3, Y2;
	Matrix dJdB1, dJdB2, dJdB3, dJdW1, dJdW2, dJdW3;
    double learningRate;
    double(*activation)(double);
    double(*activationPrime)(double);
    vector<vector<double> > inputVector, outputVector;
    vector<vector<double> > inputVectorTest, outputVectorTest;

public:
    NeuralNetwork(string filename, string filenameTest, int inputNeuron, int hiddenNeuron,
                  int outputNeuron, double rate, double(*act)(double), double(*actP)(double), bool model);
    void modelInit();
    void setRate(double rate);
    static double random(double x);
    static double stepFunction(double x);
    Matrix computeOutput(int index, vector<vector<double>> iVector);
    void learn(int index);
    void loadTraining(const string filename);
    void loadTest(const string filename);
    void train();
    void test();
    void save();
    void printInputVector(vector<double> input);
};
