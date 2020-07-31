Object-oriented neural network for recognizing handwritten digits (0-9), uses MNIST database.

Run this program (after compiling using "make") using the following format:

./neural_network

Here, ./neural_network is the executable. You will be prompted next to enter the number of iterations. The higher the number the longer it will take, but the more accurate the output will be.
Next, you will be prompted if you want to load in a previously trained network. If you choose 'y' and there is no saved network, it will simply initialize a new network.

The program will run through 100 epochs each iteration before being tested against 2000 test images. Once this is done, an overall display of
accuracy will be printed where you can see the accuracy of the neural network in guessing each digit (and the total accuracy).

We added an additional layer to help improve accuracy.
