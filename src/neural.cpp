#include "../include/matrix.h"
#include "neural.h"

// File to save trained network to
#define SAVE_FILE "saved_models/trained_model.txt"
#define IMG_WID 28
#define IMG_HGT 28

// TR_SIZE and TE_SIZE must be larger than 10
// Number of training images
#define TR_SIZE 100
// Number of test images
#define TE_SIZE 1000

// structure for integer sorting, accepted by std::sort()
struct intSorting {
    bool operator() (int i, int j) {
        return (i < j);
    }
} intSort;

NeuralNetwork::NeuralNetwork(string filenameTrain, string filenameTest, int inputNeuron, int hiddenNeuron,
                             int outputNeuron, double rate, double(*act)(double),
                             double(*actP)(double), bool model) {

    // Initialize matrices to correct sizes
    W1 = Matrix(inputNeuron, hiddenNeuron);
    W2 = Matrix(hiddenNeuron, hiddenNeuron);
    W3 = Matrix(hiddenNeuron, outputNeuron);
    B1 = Matrix(1, hiddenNeuron);
    B2 = Matrix(1, hiddenNeuron);
    B3 = Matrix(1, outputNeuron);

    // load weights from txt if load arg is passed
    if(model) {
        modelInit();
    }

    // if load arg is not passed, fill matrices with random weights
    else {
        W1 = W1.applyFunction(random);
        W2 = W2.applyFunction(random);
        W3 = W3.applyFunction(random);
        B1 = B1.applyFunction(random);
        B2 = B2.applyFunction(random);
        B3 = B3.applyFunction(random);
    }

    // initialize learning rate and activation functions
    setRate(rate);
    activation = act;
    activationPrime = actP;

    // load training and test data
    loadTraining(filenameTrain);
    loadTest(filenameTest);
}

void NeuralNetwork::modelInit() {
    fstream file(SAVE_FILE, ios::in);
    string line, data;
    Matrix matrices[6] = {W1, W2, W3, B1, B2, B3};
    if(file) {
        // loop through each matrix, initialize values based on trained_model.txt
        for(int i = 0; i < 6; ++i) {
            getline(file, line);
            stringstream s(line);

            if(getline(s, data, ' ')) {
                int hCtr;
                int wCtr;
                for(hCtr = 0; hCtr < matrices[i].getHeight(); ++hCtr) {
                    for(wCtr = 0; wCtr < matrices[i].getWidth(); ++wCtr) {
                        matrices[i].setArray(hCtr, wCtr, stod(data));
                        getline(s, data, ' ');
                    }
                }
            }
        }
    }

    W1 = *matrices;
    W2 = *(matrices + 1);
    W3 = *(matrices + 2);
    B1 = *(matrices + 3);
    B2 = *(matrices + 4);
    B3 = *(matrices + 5);
    // close the file to prevent leaks
    file.close();
}

void NeuralNetwork::setRate(double rate) {
    learningRate = rate;
}

double NeuralNetwork::random(double x) {
	// produces a random value between -.5 and +.5
    return (double)(rand() % 10000 + 1) / 10000 - 0.5;
}

double NeuralNetwork::stepFunction(double x) {
	// x can never be exactly 1 or exactly 0, so we use any value over .9 to represent a 1, and any number below .1 to represent a 0
    if(x > 0.9) {
        return 1.0;
    }
    if(x < 0.1) {
        return 0.0;
    }
    return x;
}

void NeuralNetwork::loadTraining(string filename) {
	// boundaries required for resizing the input and output vectors, so we need them to be the right size
    if((TR_SIZE >= 60000) || (TR_SIZE <= 10)) {
        cerr << "Invalid number of train epochs! Change TR_SIZE!" << endl;
        exit(1);
        return;
    }

    // make sure the vectors are cleaned out
    inputVector.clear();
    outputVector.clear();

	// resizes input and output vectors to be the size of the number of training images
    inputVector.resize(TR_SIZE);
    outputVector.resize(TR_SIZE);
    fstream file(filename, ios::in);
    if(file) {
        cout << "Generating training data..." << endl;
        string line, data;
        int n, lineCtr = 0;
        vector<int> lineNumbers;

        // randomly select data points to use for training
        for(int i = 0; i < TR_SIZE; ++i) {
            n = rand() % 60000;

            unsigned int j = 0;
            // check for duplicated
            while(j < lineNumbers.size()) {
                if(lineNumbers[j] == n) {
                    n = rand() % 60000;
                    j = 0;
                }
                else {
                    ++j;
                }
            }
            lineNumbers.push_back(n);
        }



        // sort line numbers
        sort(lineNumbers.begin(), lineNumbers.end(), intSort);

        // file processing
        for(int k = 0; k < 60000; ++k) {
            getline(file, line);
            if(k == lineNumbers[lineCtr]) {
                // stringsteam so we can get individual lines
                stringstream s(line);

                if(getline(s, data, ',')) {
                    outputVector[lineCtr].resize(10);
    				// converts character into an int
                    n = atoi(line.substr(0,1).c_str());
                    outputVector[lineCtr][n] = 1;
                }

                while(getline(s, data, ',')) {
    				// if the pixel is dark enough (greater than the shade value at 100) then replace it with a 1
                    if(stoi(data) > 100) {
                        inputVector[lineCtr].push_back(1);
                    }
                    else {
    					// if the pixel is too light, add a 0
                        inputVector[lineCtr].push_back(0);
                    }
                }
                lineCtr++;
                if(lineCtr == TR_SIZE) {
                    break;
                }
            }
        }
    //close the file to prevent leaks
    file.close();
    }
	else {
        cout << "Could not read train file!" << endl;
        cerr << "Input should be in the form <executable> <path to training data> <path to test data>" << endl;
        exit(1);
	}
}

void NeuralNetwork::loadTest(string filename) {
	// boundaries required for resizing the input and output vectors, so we need them to be the right size
    if((TE_SIZE > 10000) || (TE_SIZE < 10)) {
        cerr << "Invalid number of test epochs! Change TE_SIZE!" << endl;
        exit(1);
        return;
    }

	// resizes vectors to be the same size as the number of test images
    inputVectorTest.resize(TE_SIZE);
    outputVectorTest.resize(TE_SIZE);
    fstream file(filename, ios::in);

	// ensures file exists
    if(file) {
		string line;
		string data;
        int n;

        // file processing
        for(int i = 0; i < TE_SIZE; i++) {
            getline(file, line);

            stringstream s(line);

            if(getline(s, data, ',')) {
                outputVectorTest[i].resize(10);
                n = atoi(line.substr(0,1).c_str());
                outputVectorTest[i][n] = 1;
            }

            while(getline(s, data, ',')) {
				// if a pixel is dark enough (value > 100) then add a 1 to represent a pixel where the number is in the image
                if(stoi(data) > 100) {
                    inputVectorTest[i].push_back(1);
                }
                else {
					// otherwise add a 0
                    inputVectorTest[i].push_back(0);
                }
            }
        }
        //closes the file to prevent leaks
        file.close();
    }
	else {
        cout << "Could not read test file!" << endl;
        cerr << "Input should be in the form <executable> <path to training data> <path to test data>" << endl;
        exit(1);
    }
}

Matrix NeuralNetwork::computeOutput(int index, vector<vector<double>> iVector) {
	// creates another matrix from the input vector. Surrounding the input with curly braces {input} creates a double vector
    vector<double> input = iVector[index];
    X = Matrix({input});
    
    // starts forward-feeding process with 2 hidden layers and an output layer
    H1 = X.dot(W1).add(B1).applyFunction(activation);
    H2 = H1.dot(W2).add(B2).applyFunction(activation);
    Y = H2.dot(W3).add(B3).applyFunction(activation);
    return Y;
}

void NeuralNetwork::learn(int index) {
    vector<double> expectedOutput = outputVector[index];
    Y2 = Matrix({expectedOutput});

	// computes bias gradients
    dJdB3 = Y.subtract(Y2).multiply(H2.dot(W3).add(B3).applyFunction(activationPrime));
    dJdB2 = dJdB3.dot(W3.transpose()).multiply(H1.dot(W2).add(B2).applyFunction(activationPrime));
    dJdB1 = dJdB2.dot(W2.transpose()).multiply(X.dot(W1).add(B1).applyFunction(activationPrime));

    // compute weight gradients
    dJdW3 = H2.transpose().dot(dJdB3);
    dJdW2 = H1.transpose().dot(dJdB2);
    dJdW1 = X.transpose().dot(dJdB1);

	// these update the weights
    W1 = W1.subtract(dJdW1.multiply(learningRate));
    W2 = W2.subtract(dJdW2.multiply(learningRate));
    W3 = W3.subtract(dJdW3.multiply(learningRate));
    B1 = B1.subtract(dJdB1.multiply(learningRate));
    B2 = B2.subtract(dJdB2.multiply(learningRate));
    B3 = B3.subtract(dJdB3.multiply(learningRate));
}

void NeuralNetwork::train() {
	// iterates for ever training image
    for(int i = 0; i < TR_SIZE; ++i) {
        for(unsigned int j = 0; j < inputVector.size(); ++j) {
			// computes the output for every index
            computeOutput(j, inputVector);
			// learn from each index
            learn(j);
        }
		// each time through it gets more accurate
        cout << "Epoch #" << i+1 << "/" << TR_SIZE << endl;
    }
	// saves the status of the network
    save();
}

void NeuralNetwork::test() {
    cout << "Testing..." << endl;
    vector<int> actual;
    vector<int> prediction;

    vector<vector<double>> probabilities;

    // finds the actual value of the digit
    for(unsigned int i = 0; i < inputVectorTest.size(); ++i) {
        // set actual value of digit
        for(int j = 0; j < 10; ++j) {
            if(outputVectorTest[i][j] == 1) {
                actual.push_back(j);
            }
        }

        probabilities = computeOutput(i, inputVectorTest).applyFunction(stepFunction).getArray();
        double max = 0;
        int index = 0;
        // find prediction value of digit
        for(int i = 0; i < 10; ++i) {
            if(probabilities[0][i] >= max){
                max = probabilities[0][i];
                index = i;
            }
        }
        prediction.push_back(index);
    }

    // For computing accuracy of each seperate digit 0-9
    int val;
    int guess;
    int correct[10];
    int total[10];
    // Initialize vectors to 0
    for(int i = 0; i < 10; ++i){
        correct[i] = 0;
        total[i] = 0;
    }
    int full_total = 0;
    int full_correct = 0;
    for(unsigned int i = 0; i < inputVectorTest.size(); ++i) {
        val = actual[i];
        guess = prediction[i];
		// if the value is the same as the guess
        if(val == guess) {
			// increments the number of correct vals by 1
            correct[val] = (correct[val] + 1);
            full_correct++;
		}
		// keeps track of total number of vals so it can be compared to correct values
        total[val] = (total[val] + 1);
        full_total++;
    }

    cout << endl << "--- Accuracy ---" << endl;
    double acc;
    for(int i = 0; i < 10; ++i){
        if(total[i] == 0){
            acc = -1;
        } else {
            // accuracy is the number of correct values divided by the total number of values
            acc = correct[i] / (double)total[i];
        }
        if(acc == -1) {
            printf("No instance of %d in test subset\n", i);
        } else {
            printf("Accuracy for %d: %0.2f%%\n\n", i, (acc*100));
        }
    }
    acc = full_correct / (double) full_total;
    printf("\nOverall accuracy %0.2f%%\n", (acc*100));
}

// saves the weights by looping through each matrix and writing data to a file
void NeuralNetwork::save() {
    cout << "Saving..." << endl;
    ofstream file;
    Matrix matrices[6] = {W1, W2, W3, B1, B2, B3};
    file.open(SAVE_FILE);
    if(file) {
        // for each weight matrix
        for(int i = 0; i < 4; ++i) {
            Matrix M = matrices[i];
            int hCtr;
            int wCtr;
            for (hCtr = 0; hCtr < M.getHeight(); ++hCtr) {
        		for (wCtr = 0; wCtr < M.getWidth(); ++wCtr) {
        			file << M.getArray()[hCtr][wCtr];
        			file << " ";
        		}
        	}
            file << endl;
        }
    }
    cout << "Saved." << endl;
}

void NeuralNetwork::printInputVector(vector<double> input) {
    for(unsigned int i = 0; i < input.size(); ++i) {
        cout << input[i] << " ";
    }
    cout << endl;
}
