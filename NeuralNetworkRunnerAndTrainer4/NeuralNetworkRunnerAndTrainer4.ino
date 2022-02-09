//For RAM tracking
#include "mbed.h"
#include "mbed_mem_trace.h"

//For the SD card
#include <SPI.h>
#include <SD.h>
File mySDFile;

//For the IMU
#include <Arduino_LSM9DS1.h>

//For the BLE HID
#include "Nano33BleHID.h"
#include "signal_utils.h"
Nano33BleKeyboard bleKb("Swing Ring");

/**
 * Objective 5
 * The Queue class implements the queue ADT
 */
class Queue {
  public:
    int queueSize = 0, queueLength = 0, rearPointer = 0, frontPointer = 0;
    float* data;

    Queue(int queueSize_) {
      constructor(queueSize_);
    }

    void constructor(int queueSize_) {
      queueSize = queueSize_;
      checkAvailableMemory();
      data = (float*)calloc(queueSize, sizeof(float)); //objective 5.a.i
      for (int i = 0; i < queueSize; i++) data[i] = 0;
    }

    void checkAvailableMemory() { //objective 5.a.ii
      if(sizeof(float)*queueSize > freeMemory()) { 
        while(1);
      }
    }

    void enQueue(float dataItem) { //objective 5.b.i
      if (!Full()) {
        data[rearPointer] = dataItem;
        rearPointer = (rearPointer + 1) % queueSize;
        queueLength++;
      } else Serial.println("Already Full ");
    }

    /**
     * This method adds an array of values to the end of the queue
     */
    void enQueue(float dataArray[], int arrayLength) {
      if(arrayLength <= queueSize - queueLength) {
        for(int i = 0; i < arrayLength; i++) {
          enQueue(dataArray[i]);
        }
      }
      else Serial.println("Queue won't fit the array");
    }

    float deQueue() { ////objective 5.b.ii
      if (!Empty()) {
        float valueToReturn = data[frontPointer];
        //data[frontPointer] = 0;//let's not delete the item just in case
        frontPointer = (frontPointer + 1) % queueSize;
        queueLength--;
        return valueToReturn;
      } else {
        Serial.println("Already Empty ");
        return 0;
      }
    }

    /**
     * This method removes n elements from the front of the queue
     */
    void deQueue(int n) {
      if (queueLength >= n) {
        for(int i = 0; i < n; i++) deQueue();
      } else Serial.println("Not enough elements to deQueue");
    }

    boolean Full() { //objective 5.b.iii
      return queueLength == queueSize;
    }

    boolean Empty() { //objective 5.b.iv
      return queueLength == 0;
    }

    float Data(int i) { //objective 5.c.i
      int index = (frontPointer + i) % queueSize;
      return data[index];
    }

    void Reset() { //objective 5.c.ii
      rearPointer = 0;
      frontPointer = 0;
      for (int i = 0; i < queueSize; i++) {
        data[i] = 0;
      }
    }

    float maximum() { //objective 5.c.iii
      if (!Empty()) {
        float maximum = data[frontPointer];
        for (int i = 1; i < queueLength; i++) {
          int index = (frontPointer + i) % queueSize;
          if (data[index] > maximum) maximum = data[index];
        }
        return maximum;
      }
      else {
        Serial.println("Cannot find maximum - Empty Queue");
        return 0;
      }
    }

    float minimum() { //objective 5.c.iv
      if (!Empty()) {
        float minimum = data[frontPointer];
        for (int i = 1; i < queueLength; i++) {
          int index = (frontPointer + i) % queueSize;
          if (data[index] < minimum) minimum = data[index];
        }
        return minimum;
      }
      else {
        Serial.println("Cannot find minimum - Empty Queue");
        return 0;
      }
    }


    void Fill(float v) { //objective 5.c.v
      rearPointer = 0;
      frontPointer = 0;
      queueLength = queueSize;
      for (int i = 0; i < queueSize; i++) {
        data[i] = v;
      }
    }

    float sum() { //objective 5.c.vi
      if (!Empty()) {
        float sum = 0;
        for (int i = 0; i < queueLength; i++) {
          int index = (frontPointer + i) % queueSize;
          sum += data[index];
        }
        return sum;
      }
      else {
        Serial.println("Cannot find sum - Empty Queue");
        return 0;
      }
    }

    float average() { //objective 5.c.vii
      if (!Empty()) {
        return sum() / queueLength;
      }
      else {
        Serial.println("Cannot find average - Empty Queue");
        return 0;
      }
    }

    void remove() { //objective 5.a.iii
      free(data);
    }

    void printOut() { //objective 5.c.viii
      Serial.print("Queue =\n{");
      for (int i = 0; i < queueLength; i++) {
        int index = (frontPointer + i) % queueSize;
        Serial.print(String(data[index]));
        if(i < queueLength-1) Serial.print(",");
      }
      Serial.println("};");
    }


};

/**
 * Objective 6
 * The Function class implements "higher-order functions"
 */
class Function {
  public:
    int index;
    Function(int i) {
      index = i; //objective 6.a
    }
    static float apply(float N, float parameters[], int i, int j, int Index) {
      float result = 0;
      switch (Index) { //objective 6.b
        case 0:
          result = sigmoid(N, parameters, i, j);
          break;
        case 1:
          result = dSigmoid(N, parameters, i, j);
          break;
        default:
          break;
      }
      return result;
    }

    static float sigmoid(float N, float parameters[], int i, int j) { //objective 6.c
      float e = 2.71828;
      float y;
      y = 1.0 / (1.0 + pow(e, -N));
      return y;
    }

    static float dSigmoid(float N, float parameters[], int i, int j) { //objective 6.d
      return N * (1 - N);
    }
};

Function sigmoid(0);
Function dSigmoid(1);

/**
 * Objective 4
 * The Array class implements an array object
 */
class Array {
  public:
    String name = "array";
    int length = -1;
    float* data;

    Array() {}

    Array(String name_) {
      name = name_;
    }

    Array(int length_) { //objective 4.b.i
      constructor(length_);
    }

    Array(float a[], int arrayLength) { //objective 4.b.ii
      constructor(a, arrayLength);
    }

    Array(Array* a) { //objective 4.b.iii
      if (length != -1)remove();
      length = a->length;
      checkAvailableMemory();
      data = (float*)calloc(length, sizeof(float));
      for (int i = 0; i < length; i++) data[i] = a->data[i];
    }

    Array(Queue* q) { //objective 4.b.iv
      length = q->queueLength;
      checkAvailableMemory();
      data = (float*)calloc(length, sizeof(float));
      for (int i = 0; i < length; i++) data[i] = q->Data(i);
    }

    Array(float** matrixData, int matrixRows, int matrixCols, String name_) { //objective 4.b.v
      length = matrixRows * matrixCols;
      checkAvailableMemory();
      data = (float*)calloc(length, sizeof(float));
      name = name_;

      for (int i = 0; i < matrixRows; i++) {
        for (int j = 0; j < matrixCols; j++) {
          data[i * matrixCols + j] = matrixData[i][j];
        }
      }
    }

    void constructor(int length_) { //objective 4.b.i
      length = length_;
      checkAvailableMemory();
      data = (float*)calloc(length, sizeof(float)); //objective 4.a.i
      for (int i = 0; i < length; i++) data[i] = 0;
    }

    void constructor(float a[], int arrayLength) { //objective 4.b.ii
      length = arrayLength;
      checkAvailableMemory();
      data = (float*)calloc(length, sizeof(float));
      for (int i = 0; i < length; i++) data[i] = a[i];
    }

    void setTo(Array* a) { //objective 4.c.i
      if (length != -1)remove();
      length = a->length;
      checkAvailableMemory();
      data = (float*)calloc(length, sizeof(float));
      for (int i = 0; i < length; i++) data[i] = a->data[i];
    }

    void checkAvailableMemory() { //objective 4.a.ii
      if(sizeof(float)*length > freeMemory()) {
        while(1);
      }
    }

    void setTo(float a[], int arrayLength) { //objective 4.c.ii
      if (length != -1)remove();
      constructor(a, arrayLength);
    }

    void remove() { //objective 4.a.iii
      free(data);
    }

    void setName(String name_) {
      name = name_;
    }

    void printOut() { //objective 4.c.iii
      Serial.print("float " + name + " [] = \n{");
      for (int i = 0; i < length; i++) {
        Serial.print(data[i]);
        if (i < length - 1) Serial.print(", ");
      }
      Serial.println("};");
    }

    
};

Array fitInputArray;
Array fitOutputArray;

/**
 * Objective 3
 * The Matrix class implements a matrix object
 */
class Matrix {
  public:
    int rows = -1, cols = -1;
    float** data;
    String name = "matrix";

    Matrix() {}

    Matrix(String name_) {
      name = name_;
    }

    Matrix(int rows_, int cols_) { //objective 3.b.i
      constructor(rows_, cols_, name);
    }
 
    Matrix(int rows_, int cols_, String name_) { //objective 3.b.i
      constructor(rows_, cols_, name_);
    }

    Matrix(Matrix* a, String name_) { //objective 3.b.ii
      name = name_;
      setTo(a);
    }

    Matrix(Array* a, String name_) { //objective 3.b.iii
      name = name_;
      setTo(a);
    }

    void constructor(int rows_, int cols_) { 
      constructor(rows_, cols_, name);
    }

    void constructor(int rows_, int cols_, String name_) { 
      rows = rows_;
      cols = cols_;
      checkAvailableMemory();
      //objective 3.a.i
      data = (float**)calloc(rows, sizeof(float*));
      for (int i = 0; i < rows; i++) {
        float* myArray = (float*)calloc(cols, sizeof(float));
        data[i] = myArray;
      }
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = 0;
      name = name_;
    }

    void checkAvailableMemory() { //objective 3.a.ii
      if(sizeof(float)*rows*cols > freeMemory()) {
        while(1);
      }
    }

    void setTo(Matrix* a) { //objective 3.d.i
      if (rows != -1) {
        for (int i = 0; i < rows; i++) free(data[i]);
        free(data);
      }
      constructor(a->rows, a->cols);
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = a->data[i][j];
    }

    void setTo(Array* a) { //objective 3.d.ii
      if (rows != -1) {
        for (int i = 0; i < rows; i++) free(data[i]);
        free(data);
      }
      constructor(a->length, 1);
      for (int i = 0; i < a->length; i++) data[i][0] = a->data[i];
    }

    /**
     * This method replaces a row inthe matrix with values from an Array
     */
    void arrayToRow(int row, float myArray[]) {
      for (int i = 0; i < rows; i++) data[row][i] = myArray[i];
    }

    void remove() { //objective 3.a.iii
      if (rows != -1) {
        for (int i = 0; i < rows; i++) free(data[i]); //freeing every line's RAM
        free(data); //freeing RAM where the lines' pointers were stored
      }
    }

    void setName(String name_) { 
      name = name_;
    }

    /**
     * These methods fill a Matrix with random values
     */
    void randomise() { 
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = myRandom(-1, 1, 6);
    }

    void randomise(float first, float last, int precision) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = myRandom(first, last, precision);
    }
    
    void multiply(float scaler) { //objective 3.c.ii
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] *= scaler;
    }

    void multiply(Matrix* a) { //objective 3.c.v
      if (rows != a-> rows || cols != a-> cols) {
        Serial.println("Size mismatch in add(Matrix* a): " + info() + " " + a->info());
        while (true) {};
      }
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] *= a->data[i][j];
    }

    void add(float add) { //objective 3.c.i
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] += add;
    }

    void add(Matrix* a) { //objective 3.c.iii
      if (rows != a-> rows || cols != a-> cols) {
        Serial.println("Size mismatch in add(Matrix* a): " + info() + " " + a->info());
        while (true) {}
      }
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] += a->data[i][j];
    }

    //objective 3.d.iii
    void map(Function* f, float parameters[]) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = f->apply(data[i][j], parameters, i, j, f->index);
    }
    void map(Function* f, float a) { 
      float parameters[] = {a};
      map(f, parameters);
    }
    void map(Function* f) {
      float parameters[] = {};
      map(f, parameters);
    }
    void map(Matrix* M, Function* f, float parameters[]) {
      if (rows != -1)remove();
      constructor(M->rows, M->cols);
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = f->apply(M->data[i][j], parameters, i, j, f->index);
    }
    void map(Matrix* M, Function* f, float a) { //no memory loss
      float parameters[] = {a};
      map(M, f, parameters);
    }
    void map(Matrix* M, Function* f) {
      float parameters[] = {};
      map(M, f, parameters);
    }

    void MatrixMultiply(Matrix* a, Matrix* b) { //objective 3.c.vi
      if (a->cols != b->rows) {
        Serial.println("Size mismatch in MatrixMultiply(Matrix* a, Matrix* b): " + a->info() + " " + b->info());
        while (true) {};
      }
      remove();
      constructor(a->rows, b->cols);

      float newData[rows][cols];

      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          float sum = 0;
          for (int k = 0; k < a->cols; k++) {
            sum += a->data[i][k] * b->data[k][j];
          }
          newData[i][j] = sum;
        }
      }
      for (int i = 0; i < a->rows; i++) for (int j = 0; j < b->cols; j++) data[i][j] = newData[i][j];
    }
   
    void MatrixSubtract(Matrix* a, Matrix* b) { //objective 3.c.iv
      if (a->rows != b-> rows || a->cols != b-> cols) {
        Serial.println("Size mismatch in MatrixSubtract(Matrix* a, Matrix* b): " + a->info() + " " + b->info());
        while (true) {};
      }
      remove();
      constructor(a->rows, a->cols);
      for (int i = 0; i < a->rows; i++) for (int j = 0; j < a->cols; j++) data[i][j] = a->data[i][j] - b->data[i][j];
    }

    void transpose() { //objective 3.c.vii
      int oldRows = rows, oldCols = cols;
      float newData[rows][cols];
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) newData[i][j] = data[i][j];
      remove();
      constructor(oldCols, oldRows);
      for (int i = 0; i < oldRows; i++) {
        for (int j = 0; j < oldCols; j++) {
          data[j][i] = newData[i][j];
        }
      }
    }

    void matrixFromArray(Array* myArray) { //objective 3.d.iv
      if (rows != -1)remove();
      constructor(myArray->length, 1);
      for (int i = 0; i < rows; i++) {
        data[i][0] = myArray->data[i];
      }
    }

    String arrayToString(float myArray[], int arrayLength, String text) { 
      text += "{";
      for (int i = 0; i < arrayLength; i++) {
        if (i < arrayLength - 1) text += (String(myArray[i]) + ",");
        else text += (String(myArray[i]));
      }
      text += "}";
      return text;
    }

    String MatrixToString(String text) { //objective 3.d.v
      text += "{\n";
      for (int i = 0; i < rows; i++) {
        text = arrayToString(data[i], cols, text);
        if (i < rows - 1) text += ",\n";
      }
      text += "\n}";
      return text;
    }

    void printInfo() {
      Serial.println(info());
    }

    String info() {
      return name + "(" + String(rows) + ", " + String(cols) + ")";
    }

    void printOut() { //objective 3.d.vi
      Serial.println("float " + name + "(" + rows + ", " + cols + ") = {");
      for (int i = 0; i < rows; i++) {
        Serial.print("{");
        for (int j = 0; j < cols; j++) {
          Serial.print(data[i][j]);
          if (j < cols - 1) Serial.print(", ");
        }
        if (i < rows - 1) Serial.println("},");
        else  Serial.println("}");
      }
      Serial.println("};");
    }



    float myRandom(float first, float last, int precision) { //objective 3.d.vii
      precision = max(precision, 1);
      float factor = pow(10, precision - 1);
      float r = random(0, (last - first) * factor);
      r /= factor;
      r += first;
      return r;
    }

};

Matrix matrix("matrixForReference");

/**
 * Objective 2
 * The neural network class
 */
class NeuralNetwork {
  public:
    String name = "neural network";
    float learning_rate = 0.1;
    boolean successfulRead = false;
    int numberOfLayers;
    int* numberOfNeurons;
    Matrix* neurons;
    Matrix* weights;
    Matrix* biases;
    Matrix trainingInput;
    Matrix trainingOutput;
    int trainingBunch, totalTrainingSamples;
    
    NeuralNetwork() {}

    NeuralNetwork(int* NofNeurons, int numberOfLayers_) {
      constructor(NofNeurons, numberOfLayers_);
    }

    void constructor(int* NofNeurons, int numberOfLayers_) { //objective 2.c
      //define the shape of the network
      numberOfLayers = numberOfLayers_;
      numberOfNeurons = NofNeurons;

      //memory allocation for the variable size matrices
      neurons = (Matrix*)calloc(numberOfLayers, sizeof(Matrix));
      weights = (Matrix*)calloc(numberOfLayers - 1, sizeof(Matrix));
      biases = (Matrix*)calloc(numberOfLayers - 1, sizeof(Matrix));

      //initialise neurons, weights, biases
      for (int i = 0; i < numberOfLayers; i++) {
        if (i < numberOfLayers - 1) {
          weights[i].constructor(numberOfNeurons[i + 1], numberOfNeurons[i], "weights" + String(i));
          biases[i].constructor(numberOfNeurons[i + 1], 1, "biases" + String(i));

          weights[i].randomise();
          biases[i].randomise();
        }
        neurons[i].constructor(numberOfNeurons[i], 1, "neurons" + String(i));
      }
    }

    /**
     * This method creates the training data matrices
     * based on the number of training samples that are
     * wanted to be loaded at one time
     */
    void prepareForTraining(int numberOfTrainingSamplesInABunch, int totalNumberOfTrainingSamples) {
      trainingBunch = numberOfTrainingSamplesInABunch;
      totalTrainingSamples = totalNumberOfTrainingSamples;
      trainingInput.constructor(numberOfTrainingSamplesInABunch, numberOfNeurons[0]);
      trainingOutput.constructor(numberOfTrainingSamplesInABunch, numberOfNeurons[numberOfLayers-1]);
    }

    /**
     * This method loads a set of random training samples
     */
    void loadRandomTrainingSamples() {
      int fileIndexes[trainingBunch];
      for(int i = 0; i < trainingBunch; i++) fileIndexes[i] = int(myRandom(0, totalTrainingSamples-1, 1));
      loadTrainingSamples(fileIndexes);
    }

    void loadTrainingSamples(int fileIndexes[]) { //objective 2.g.iii
      successfulRead = true;
      for(int i = 0; i < trainingBunch; i++) {
        mySDFile = SD.open("TD"+String(fileIndexes[i])+".TXT");
        if(SD_retrieveNextNumber() != trainingInput.cols) successfulRead = false;
        if(SD_retrieveNextNumber() != trainingOutput.cols) successfulRead = false;
        for(int j = 0; j < trainingInput.cols && successfulRead; j++) {
          trainingInput.data[i][j] = SD_retrieveNextNumber();
        }
        for(int j = 0; j < trainingOutput.cols && successfulRead; j++) {
          trainingOutput.data[i][j] = SD_retrieveNextNumber();
        }
        mySDFile.close();
        if(!successfulRead) {
          Serial.println("Size mismatch when reading training data file " + String(fileIndexes[i]));
          while(1);
        }
      }
    }

    void forwardPropagation(Array* input_array) { //objective 2.e
      neurons[0].setTo(input_array);
      for (int i = 0; i < numberOfLayers - 1; i++) {
        neurons[i + 1].MatrixMultiply(&weights[i], &neurons[i]);
        neurons[i + 1].add(&biases[i]);
        neurons[i + 1].map(&sigmoid);
      }
    }

    /**
     * The method with back propagation to train the network
     */
    void train(Array* input_array, Array* desiredOutputs_array) { //objective 2.f.i
      //feed forward the input to calculate all neuron values
      forwardPropagation(input_array);

      Matrix previous_errors("previous_errors");
      for (int i = numberOfLayers - 1; i > 0; i--) {
        //calculate errors
        Matrix errors("errors");
        if (i == numberOfLayers - 1) {//if dealing with the last layer
          Matrix desiredOutputs_Matrix(desiredOutputs_array, "desiredOutputs_Matrix");
          errors.MatrixSubtract(&desiredOutputs_Matrix, &neurons[i]);
          desiredOutputs_Matrix.remove();
        }
        else { //if dealing with other layers
          Matrix weights_i_T(&weights[i], "weights_i_T");
          weights_i_T.transpose();
          errors.MatrixMultiply(&weights_i_T, &previous_errors);
          weights_i_T.remove();
        }

        //calculate changesToBiases
        Matrix changesToBiases("changesToBiases");
        changesToBiases.map(&neurons[i], &dSigmoid);
        changesToBiases.multiply(&errors);
        changesToBiases.multiply(learning_rate);
        //adjust biases
        biases[i - 1].add(&changesToBiases);
        //calculate deltas for weights
        Matrix changesToWeights("changesToWeights");
        Matrix neurons_im1_T(&neurons[i - 1], "neurons_im1_T");
        neurons_im1_T.transpose();
        changesToWeights.MatrixMultiply(&changesToBiases, &neurons_im1_T);
        //adjust weights
        weights[i - 1].add(&changesToWeights);

        previous_errors.setTo(&errors);

        errors.remove();
        changesToBiases.remove();
        changesToWeights.remove();
        neurons_im1_T.remove();
      }
      previous_errors.remove();
    }

    //objective 2.f.ii
    /**
     * This method trains the network
     * for epochs number of times
     */
    void fit(int epochs, int printRate) {
      for (int i = 0; i < epochs; i++) {
        int sampleN = int(myRandom(0, trainingInput.rows, 1));
        fitInputArray.setTo(trainingInput.data[sampleN], trainingInput.cols);
        fitOutputArray.setTo(trainingOutput.data[sampleN], trainingOutput.cols);
        train(&fitInputArray, &fitOutputArray);
        if (printRate != 0 && i % printRate == 0) Serial.println(10 * fullCost(&fitOutputArray));
      }
    }

    /**
     * This method trains the network until the output error
     * is consistently smaller than the threshold value for
     * acceptedIterationsGoal times in a row
     */
    void fit(int acceptedIterationsGoal, float threshold, int printRate) {
      int acceptedIterations = 0;
      for (int i = 0; acceptedIterations < acceptedIterationsGoal; i++ && acceptedIterations++) {
        int sampleN = int(myRandom(0, trainingInput.rows, 1));
        fitInputArray.setTo(trainingInput.data[sampleN], trainingInput.cols);
        fitOutputArray.setTo(trainingOutput.data[sampleN], trainingOutput.cols);
        train(&fitInputArray, &fitOutputArray);
        float cost = fullCost(&fitOutputArray);
        if (cost > threshold) acceptedIterations = 0;
        if (printRate != 0 && i % printRate == 0) {
          Serial.print(10 * cost);
          Serial.print(", ");
          Serial.println(10 * threshold);
        }
      }
    }

    float fullCost(Array* fitOutputArray) { //objective 2.f.iii
      float sum = 0;
      for (int i = 0; i < fitOutputArray->length; i++) sum += pow((fitOutputArray->data[i] - neurons[numberOfLayers - 1].data[i][0]), 2); //difference squared
      if (sum < 0) Serial.println("cost less than zero");
      return sum;
    }

    void assignRandomNetwork() { //objective 2.d
      for (int i = 0; i < numberOfLayers - 1; i++) {
        weights[i].randomise();
        biases[i].randomise();
      }
    }

    /**
     * This method prints out all of the content 
     * of the .txt file into the serial monitor
     */
    void readWholeFile(String mySDFileName) {
      mySDFile = SD.open(mySDFileName);
      if (mySDFile) {
        Serial.println(mySDFileName);
        long lastTimeCharReceived = millis();
        while (millis() - lastTimeCharReceived < 1000) {
          while (mySDFile.available()) {
            char c = mySDFile.read();
            lastTimeCharReceived = millis();
            Serial.print(c);
          }
        }
        mySDFile.close();
      } else Serial.println("error opening " + mySDFileName);
    }

    void assignFromFile(String mySDFileName) { //objective 2.g.i.3
      successfulRead = true; //temporarly (it will change to false when it has to)
      boolean sizeMismatch = false;
      int totalDataPieces = totalPiecesOfData();
      mySDFile = SD.open(mySDFileName);
      for (int dataIndex = 0; dataIndex < totalDataPieces && !sizeMismatch && successfulRead; dataIndex++) {
        float readData = SD_retrieveNextNumber();
        if (dataIndex == 0) {
          if (int(readData) != numberOfLayers) sizeMismatch = true;
        }
        else if (dataIndex < numberOfLayers + 1) {
          if (int(readData) != numberOfNeurons[dataIndex - 1])  sizeMismatch = true;
        }
        else if (dataIndex < totalPiecesOfWeightsData() + numberOfLayers + 1) {
          //weights' data falls in here
          int weightDataIndex = dataIndex - numberOfLayers - 1;
          int weightLayer = weightsArrayIndex(weightDataIndex);
          weightDataIndex -= totalPiecesOfWeightsDataUpTo(weightLayer);
          weights[weightLayer].data[weightDataIndex / numberOfNeurons[weightLayer]][weightDataIndex % numberOfNeurons[weightLayer]] = readData;
        }
        else {
          //biases' data falls in here
          int biasDataIndex = dataIndex - totalPiecesOfWeightsData() - numberOfLayers - 1;
          int biasLayer = biasesArrayIndex(biasDataIndex);
          biasDataIndex -= totalPiecesOfBiasesDataUpTo(biasLayer);
          biases[biasLayer].data[biasDataIndex % numberOfNeurons[biasLayer]][0] = readData;
        }
      }
      mySDFile.close();
      if (sizeMismatch) {
        Serial.println("Size mismatch between network in the file and initialised network");
        successfulRead = false;
        while (1);
      }
    }

    void writeToFile(String mySDFileName) { //objective 2.g.ii
      if (SD.exists(mySDFileName)) SD.remove(mySDFileName); //delete file (I then create it so basically I just clear it
      mySDFile = SD.open(mySDFileName, FILE_WRITE);
      mySDFile.print(String(numberOfLayers) + ",");
      for (int i = 0; i < numberOfLayers; i++) {
        mySDFile.print(String(numberOfNeurons[i]) + ",");
      }
      for (int l = 0; l < numberOfLayers - 1; l++) {
        for (int i = 0; i < numberOfNeurons[l + 1]; i++) {
          for (int j = 0; j < numberOfNeurons[l]; j++) {
            mySDFile.print(String(weights[l].data[i][j], 7) + ",");
          }
        }
      }
      for (int l = 0; l < numberOfLayers - 1; l++) {
        for (int i = 0; i < numberOfNeurons[l + 1]; i++) {
          mySDFile.print(String(biases[l].data[i][0], 7) + ",");
        }
      }
      mySDFile.close();
    }

    float SD_retrieveNextNumber() { //objective 2.g.i.1
      long lastTimeCharacterReceived = millis();
      boolean numberDone = false;
      float theNumber = 0;
      int charIndex = -1;
      String numberString = "";
      while ((millis() - lastTimeCharacterReceived < 1000) && !numberDone) {//waiting for acharacter for maximum of 1 second
        while (mySDFile.available() && !numberDone) {
          lastTimeCharacterReceived = millis();
          char c = mySDFile.read(); //retrieve the next character
          if (isADigit(c) || c == '-' || c == '.') numberString += c;
          else if (c == ',') {
            numberDone = true;
            theNumber = numberString.toFloat();
          }
        }
      }
      if (numberString == "") {
        Serial.println("No numbers found");
        successfulRead = false;
      }
      return theNumber;
    }

    void readNetworkFromSD() { //objective 2.g.iv.1
      assignFromFile("network.txt"); //read the saved network
      if (successfulRead) writeToFile("backup.txt"); //if successful, copy the network into backup
      else assignFromFile("backup.txt"); //if not successful, read the backup
    }

    void saveNetworkToSD() { //objective 2.g.iv.2
      writeToFile("network.txt");
      writeToFile("backup.txt");
    }

    void printOut() { //objective 2.g.v
      Serial.println(String(numberOfLayers) + ",");
      for (int i = 0; i < numberOfLayers; i++) {
        Serial.print(String(numberOfNeurons[i]) + ",");
      }
      Serial.println();
      for (int l = 0; l < numberOfLayers - 1; l++) {
        weights[l].printOut();
      }
      for (int l = 0; l < numberOfLayers - 1; l++) {
        biases[l].printOut();
      }
    }

    float myRandom(float first, float last, int precision) {
      precision = max(precision, 1);
      float factor = pow(10, precision - 1);
      float r = random(0, (last - first) * factor);
      r /= factor;
      r += first;
      return r;
    }

    boolean isADigit(char c) {
      return int(c) >= int('0') && int(c) <= int('9');
    }

    /**
     * The methods below are the implementation of the objective 2.g.i.2
     */
    int totalPiecesOfData() {
      int sum = numberOfLayers + 1;
      for (int i = 0; i < numberOfLayers - 1; i++) {
        sum += numberOfNeurons[i + 1] * (numberOfNeurons[i] + 1);
      }
      return sum;
    }

    int totalPiecesOfWeightsData() {
      int sum = 0;
      for (int i = 0; i < numberOfLayers - 1; i++) {
        sum += numberOfNeurons[i + 1] * numberOfNeurons[i];
      }
      return sum;
    }

    int totalPiecesOfWeightsDataUpTo(int upto) {
      int sum = 0;
      for (int i = 0; i < upto; i++) {
        sum += numberOfNeurons[i + 1] * numberOfNeurons[i];
      }
      return sum;
    }

    int weightsArrayIndex(int index) {
      int sum = 0;
      int i = 0;
      for (; i < numberOfLayers - 1 && sum <= index; i++) {
        sum += numberOfNeurons[i + 1] * numberOfNeurons[i];
      }
      return i - 1;
    }

    int biasesArrayIndex(int index) {
      int sum = 0;
      int i = 0;
      for (; i < numberOfLayers - 1 && sum <= index; i++) {
        sum += numberOfNeurons[i + 1];
      }
      return i - 1;
    }

    int totalPiecesOfBiasesDataUpTo(int upto) {
      int sum = 0;
      for (int i = 0; i < upto; i++) {
        sum += numberOfNeurons[i + 1];
      }
      return sum;
    }
};
