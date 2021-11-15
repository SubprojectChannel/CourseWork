//








//this one was used to figure out what was wrong with trainer2













//memory stuff
#include "mbed.h"
#include "mbed_mem_trace.h"

//For SD card
#include <SPI.h>
#include <SD.h>
File mySDFile;

//For IMU
#include <Arduino_LSM9DS1.h>


///////////////////////////////////////////////////Queue class

class Queue {
  public:
    int queueSize = 0, queueLength = 0, rearPointer = 0, frontPointer = 0;
    float* data;

    Queue(int queueSize_) {
      constructor(queueSize_);
    }

    void constructor(int queueSize_) {
      queueSize = queueSize_;
      //checkAvailableMemory();
      data = (float*)calloc(queueSize, sizeof(float));
      for (int i = 0; i < queueSize; i++) data[i] = 0;
    }

    void enQueue(float dataItem) {
      if (!Full()) {
        data[rearPointer] = dataItem;
        rearPointer = (rearPointer + 1) % queueSize;
        queueLength++;
      } else Serial.println("Already Full ");
    }

    void enQueue(float dataArray[], int arrayLength) {
      if(arrayLength <= queueSize - queueLength) {
        for(int i = 0; i < arrayLength; i++) {
          enQueue(dataArray[i]);
        }
      }
      else Serial.println("Queue won't fit the array");
    }

    float deQueue() {
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

    void deQueue(int n) {
      if (queueLength >= n) {
        for(int i = 0; i < n; i++) deQueue();
      } else Serial.println("Not enough elements to deQueue");
    }

    float Data(int i) {
      int index = (frontPointer + i) % queueSize;
      return data[index];
    }

    boolean Full() {
      return queueLength == queueSize;
    }

    boolean Empty() {
      return queueLength == 0;
    }

    void Reset() {
      rearPointer = 0;
      frontPointer = 0;
      for (int i = 0; i < queueSize; i++) {
        data[i] = 0;
      }
    }

    float maximum() {
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

    float minimum() {
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


    void Fill(float v) {
      rearPointer = 0;
      frontPointer = 0;
      queueLength = queueSize;
      for (int i = 0; i < queueSize; i++) {
        data[i] = v;
      }
    }

    float sum() {
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

    float average() {
      if (!Empty()) {
        return sum() / queueLength;
      }
      else {
        Serial.println("Cannot find average - Empty Queue");
        return 0;
      }
    }

    void remove() {
      free(data);
    }

    void printOut() {
      Serial.print("Queue =\n{");
      for (int i = 0; i < queueLength; i++) {
        int index = (frontPointer + i) % queueSize;
        Serial.print(String(data[index]));
        if(i < queueLength-1) Serial.print(",");
      }
      Serial.println("};");
    }


};


///////////////////////////////////////////////Function class

class Function {
  public:
    int index;
    Function(int i) {
      index = i;
    }
    static float apply(float N, float parameters[], int i, int j, int Index) {
      float result = 0;
      switch (Index) {
        case 0: //double
          result = sigmoid(N, parameters, i, j);
          break;
        case 1: //half
          result = dSigmoid(N, parameters, i, j);
          break;
        case 2:
          result = multiply(N, parameters, i, j);
          break;
        default:
          break;
      }
      return result;
    }

    static float sigmoid(float N, float parameters[], int i, int j) {
      float e = 2.71828;
      float y;
      y = 1.0 / (1.0 + pow(e, -N));
      return y;
    }

    static float dSigmoid(float N, float parameters[], int i, int j) {
      return N * (1 - N);
    }

    static float multiply(float N, float parameters[], int i, int j) {
      float scaler = parameters[0];
      return N * scaler;
    }
};

Function sigmoid(0);
Function dSigmoid(1);
Function multiply(2);

class Array {
  public:
    String name = "array";
    int length = -1;
    float* data;

    Array() {}

    Array(String name_) {
      name = name_;
    }

    Array(int length_) {
      constructor(length_);
    }

    Array(float a[], int arrayLength) {
      constructor(a, arrayLength);
    }

    Array(Array* a) {
      if (length != -1)remove();
      length = a->length;
      data = (float*)calloc(length, sizeof(float));
      for (int i = 0; i < length; i++) data[i] = a->data[i];
    }

    Array(float** matrixData, int matrixRows, int matrixCols, String name_) { //no memory loss
      length = matrixRows * matrixCols;
      data = (float*)calloc(length, sizeof(float));
      name = name_;

      for (int i = 0; i < matrixRows; i++) {
        for (int j = 0; j < matrixCols; j++) {
          data[i * matrixCols + j] = matrixData[i][j];
        }
      }
    }

    void constructor(int length_) {
      length = length_;
      data = (float*)calloc(length, sizeof(float));
      for (int i = 0; i < length; i++) data[i] = 0;
    }

    void constructor(float a[], int arrayLength) {
      length = arrayLength;
      data = (float*)calloc(length, sizeof(float));
      for (int i = 0; i < length; i++) data[i] = a[i];
    }

    void setTo(Array* a) {
      if (length != -1)remove();
      length = a->length;
      data = (float*)calloc(length, sizeof(float));
      for (int i = 0; i < length; i++) data[i] = a->data[i];
    }

    void setTo(float a[], int arrayLength) {
      if (length != -1)remove();
      constructor(a, arrayLength);
    }

    void remove() {
      free(data);
    }

    void setName(String name_) {
      name = name_;
    }

    void  matrixToArray(float** matrixData, int matrixRows, int matrixCols) { //no memory loss
      if (length != -1) free(data);
      length = matrixRows * matrixCols;
      data = (float*)calloc(length, sizeof(float));

      for (int i = 0; i < matrixRows; i++) {
        for (int j = 0; j < matrixCols; j++) {
          data[i * matrixCols + j] = matrixData[i][j];
        }
      }
    }

    void printOut() { //checked
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

/////////////////////////////////////////////////////////////////////  MATRIX
class Matrix {
  public:
    int rows = -1, cols = -1;
    float** data;
    String name = "matrix";

    Matrix() {}

    Matrix(String name_) {
      name = name_;
    }
    /*
        Matrix(int rows_, int cols_) { //checked
          constructor(rows_, cols_, name);
        }
    */
    Matrix(int rows_, int cols_, String name_) { //checked
      constructor(rows_, cols_, name_);
    }

    Matrix(Matrix* a, String name_) {
      name = name_;
      setTo(a);
    }

    Matrix(Array* a, String name_) {
      name = name_;
      setTo(a);
    }

    void constructor(int rows_, int cols_) { //checked
      constructor(rows_, cols_, name);
    }

    void constructor(int rows_, int cols_, String name_) { //checked
      rows = rows_;
      cols = cols_;
      data = (float**)calloc(rows, sizeof(float*));
      for (int i = 0; i < rows; i++) {
        float* myArray = (float*)calloc(cols, sizeof(float));
        data[i] = myArray;
      }
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = 0;
      name = name_;
    }

    void setTo(Matrix* a) { //checked
      if (rows != -1) {
        for (int i = 0; i < rows; i++) free(data[i]);
        free(data);
      }
      constructor(a->rows, a->cols);
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = a->data[i][j];
    }

    void setTo(Array* a) { //checked
      if (rows != -1) {
        for (int i = 0; i < rows; i++) free(data[i]);
        free(data);
      }
      constructor(a->length, 1);
      for (int i = 0; i < a->length; i++) data[i][0] = a->data[i];
    }


    void arrayToRow(int row, float myArray[]) {
      for (int i = 0; i < rows; i++) data[row][i] = myArray[i];
    }

    void remove() { //checked
      if (rows != -1) {
        for (int i = 0; i < rows; i++) free(data[i]);
        free(data);
      }
    }

    void setName(String name_) { //checked
      name = name_;
    }

    void randomise() { //checked
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = myRandom(-1, 1, 6);
    }

    void randomise(float first, float last, int precision) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = myRandom(first, last, precision);
    }

    void multiply(float scaler) { //checked
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] *= scaler;
    }

    void multiply(Matrix* a) {
      if (rows != a-> rows || cols != a-> cols) {
        Serial.println("Size mismatch in add(Matrix* a): " + info() + " " + a->info());
        while (true) {};
      }
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] *= a->data[i][j];
    }

    void add(float add) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] += add;
    }

    void add(Matrix* a) {
      if (rows != a-> rows || cols != a-> cols) {
        Serial.println("Size mismatch in add(Matrix* a): " + info() + " " + a->info());
        while (true) {}
      }
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] += a->data[i][j];
    }

    void map(Function* f, float parameters[]) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = f->apply(data[i][j], parameters, i, j, f->index);
    }
    void map(Function* f, float a) { //checked
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

    void MatrixMultiply(Matrix* a, Matrix* b) { //no nenory loss
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

    void MatrixSubtract(Matrix* a, Matrix* b) { //no memory loss
      if (a->rows != b-> rows || a->cols != b-> cols) {
        Serial.println("Size mismatch in MatrixSubtract(Matrix* a, Matrix* b): " + a->info() + " " + b->info());
        while (true) {};
      }
      remove();
      constructor(a->rows, a->cols);
      for (int i = 0; i < a->rows; i++) for (int j = 0; j < a->cols; j++) data[i][j] = a->data[i][j] - b->data[i][j];
    }

    void transpose() { //no memory loss
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

    void matrixFromArray(Array* myArray) {
      if (rows != -1)remove();
      constructor(myArray->length, 1);
      for (int i = 0; i < rows; i++) {
        data[i][0] = myArray->data[i];
      }
    }

    String arrayToString(float myArray[], int arrayLength, String text) { //checked
      text += "{";
      for (int i = 0; i < arrayLength; i++) {
        if (i < arrayLength - 1) text += (String(myArray[i]) + ",");
        else text += (String(myArray[i]));
      }
      text += "}";
      return text;
    }

    String MatrixToString(String text) { //checked
      text += "{\n";
      for (int i = 0; i < rows; i++) {
        text = arrayToString(data[i], cols, text);
        if (i < rows - 1) text += ",\n";
      }
      text += "\n}";
      return text;
    }
    /*
        void printOut() { //checked
          String firstLine = "float " + name + " [][] = ";
          Serial.println(MatrixToString(firstLine) + ";\n");
        }
    */
    void printInfo() {
      Serial.println(info());
    }

    String info() {
      return name + "(" + String(rows) + ", " + String(cols) + ")";
    }

    void printOut() { //checked
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



    float myRandom(float first, float last, int precision) { //checked
      precision = max(precision, 1);
      float factor = pow(10, precision - 1);
      float r = random(0, (last - first) * factor);
      r /= factor;
      r += first;
      return r;
    }

};

Matrix matrix("matrixForReference");


//////////////////////////////////////////////////////////////// NEURAL NETWORK
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

    void constructor(int* NofNeurons, int numberOfLayers_) {
      //define shape of the network
      numberOfLayers = numberOfLayers_;
      numberOfNeurons = NofNeurons;

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

    void prepareForTraining(int numberOfTrainingSamplesInABunch, int totalNumberOfTrainingSamples) {
      trainingBunch = numberOfTrainingSamplesInABunch;
      totalTrainingSamples = totalNumberOfTrainingSamples;
      trainingInput.constructor(numberOfTrainingSamplesInABunch, numberOfNeurons[0]);
      trainingOutput.constructor(numberOfTrainingSamplesInABunch, numberOfNeurons[numberOfLayers-1]);
    }

    void loadTrainingSamples(int fileIndexes[]) {
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

    void forwardPropagation(Array* input_array) {
      neurons[0].setTo(input_array);
      for (int i = 0; i < numberOfLayers - 1; i++) {
        neurons[i + 1].MatrixMultiply(&weights[i], &neurons[i]);
        neurons[i + 1].add(&biases[i]);
        neurons[i + 1].map(&sigmoid);
      }
    }

    void train(Array* input_array, Array* desiredOutputs_array) {
      //feed forward the input to calculate all neuron values
      forwardPropagation(input_array);

      Matrix previous_errors("previous_errors");
      for (int i = numberOfLayers - 1; i > 0; i--) {
        //calculate errors
        Matrix errors("errors");
        if (i == numberOfLayers - 1) {
          Matrix desiredOutputs_Matrix(desiredOutputs_array, "desiredOutputs_Matrix");
          errors.MatrixSubtract(&desiredOutputs_Matrix, &neurons[i]);
          desiredOutputs_Matrix.remove();
        }
        else {
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

    void fit(Matrix* inputsMatrix, Matrix* outputsMatrix, int epochs, int printRate) {
      for (int i = 0; i < epochs; i++) {
        int sampleN = int(myRandom(0, inputsMatrix->rows, 1));
        fitInputArray.setTo(inputsMatrix->data[sampleN], inputsMatrix->cols);
        fitOutputArray.setTo(outputsMatrix->data[sampleN], outputsMatrix->cols);
        train(&fitInputArray, &fitOutputArray);
        if (printRate != 0 && i % printRate == 0) Serial.println(10 * fullCost(&fitOutputArray));
      }
    }

    void fit(Matrix* inputsMatrix, Matrix* outputsMatrix, int acceptedIterationsGoal, float treshold, int printRate) {
      int acceptedIterations = 0;
      for (int i = 0; acceptedIterations < acceptedIterationsGoal; i++ && acceptedIterations++) {
        int sampleN = int(myRandom(0, inputsMatrix->rows, 1));
        fitInputArray.setTo(inputsMatrix->data[sampleN], inputsMatrix->cols);
        fitOutputArray.setTo(outputsMatrix->data[sampleN], outputsMatrix->cols);
        train(&fitInputArray, &fitOutputArray);
        float cost = fullCost(&fitOutputArray);
        if (cost > treshold) acceptedIterations = 0;
        if (printRate != 0 && i % printRate == 0) {
          Serial.print(10 * cost);
          Serial.print(", ");
          Serial.println(10 * treshold);
        }
      }
    }

    float fullCost(Array* fitOutputArray) {
      float sum = 0;
      for (int i = 0; i < fitOutputArray->length; i++) sum += pow((fitOutputArray->data[i] - neurons[numberOfLayers - 1].data[i][0]), 2);
      if (sum < 0) Serial.println("cost less than zero");
      return sum;
    }

    void assignRandomNetwork() {
      for (int i = 0; i < numberOfLayers - 1; i++) {
        weights[i].randomise();
        biases[i].randomise();
      }
    }

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

    void assignFromFile(String mySDFileName) {
      successfulRead = true; //temporarly (it will change to false when it has to
      boolean sizeMismatch = false;
      int totalDataPieces = totalPiecesOfData();
      mySDFile = SD.open(mySDFileName);
      //Serial.println(totalPiecesOfWeightsData(numberOfLayers, numberOfNeurons));
      for (int dataIndex = 0; dataIndex < totalDataPieces && !sizeMismatch && successfulRead; dataIndex++) {
        float readData = SD_retrieveNextNumber();
        //Serial.println(dataIndex);
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
          //Serial.println("weights" + String(weightLayer) + "[" + String(weightDataIndex / numberOfNeurons[weightLayer]) + "][" + String(weightDataIndex % numberOfNeurons[weightLayer]) + "]=" + String(data, 7));
          weights[weightLayer].data[weightDataIndex / numberOfNeurons[weightLayer]][weightDataIndex % numberOfNeurons[weightLayer]] = readData;
        }
        else {
          //biases' data falls in here
          int biasDataIndex = dataIndex - totalPiecesOfWeightsData() - numberOfLayers - 1;
          int biasLayer = biasesArrayIndex(biasDataIndex);
          biasDataIndex -= totalPiecesOfBiasesDataUpTo(biasLayer);
          //Serial.println("biases" + String(biasLayer) + "[" + String(biasDataIndex % numberOfNeurons[biasLayer]) + "]=" + String(data, 7));
          biases[biasLayer].data[biasDataIndex % numberOfNeurons[biasLayer]][0] = readData;
        }


        //if(float(float(int(data1))) == data1) Serial.print(String(int(data1+1)) + ",");
        //else Serial.print(String(data1+1, 7) + ",");
      }
      mySDFile.close();
      if (sizeMismatch) {
        Serial.println("Size mismatch between network in the file and initialised network");
        successfulRead = false;
        while (1);
      }
    }

    void writeToFile(String mySDFileName) {
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

    float SD_retrieveNextNumber() {
      long lastTimeCharacterReceived = millis();
      boolean numberDone = false;
      float theNumber = 0;
      int charIndex = -1;
      String numberString = "";
      while ((millis() - lastTimeCharacterReceived < 1000) && !numberDone) {
        while (mySDFile.available() && !numberDone) {
          lastTimeCharacterReceived = millis();
          char c = mySDFile.read();
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

    void readNetworkFromSD() {
      assignFromFile("network.txt"); //read the saved network
      if (successfulRead) writeToFile("backup.txt"); //if successful, copy the network into backup
      else assignFromFile("backup.txt"); //if not successful, read the backup
    }

    void saveNetworkToSD() {
      writeToFile("network.txt");
      writeToFile("backup.txt");
    }

    void printOut() {
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


    ///////////////////////////////////////////////////////////////////////ew
    boolean isADigit(char c) {
      return int(c) >= int('0') && int(c) <= int('9');
    }

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







//////////////////////////////////////////////////////////////////////////Functions

/*
  String initialisePointers(Matrix a[], String pointerName) {
  String text = "";
  text += "char* " + pointerName + "[] = new char*[" + a.length + "];\n";
  return text;
  }

  String assignPointers(Matrix a[], String pointerName) {
  String text = "";
  for (int i = 0; i < a.length; i++) {
    text += pointerName + "[" + i + "] = " + a[i].name + ";\n";
  }
  return text;
  }

  String definePointers(Matrix a[], String pointerName) {
  String text = "";
  text += "char* " + pointerName + "[] = {";
  for (int i = 0; i < a.length; i++) {
    text += a[i].name;
    if (i < a.length - 1) text += ", ";
  }
  text+= "};\n";
  return text;
  }



  NeuralNetwork fileToNeuralNetwork(String fileName) {
  //extract text
  String extractedText[] = loadStrings(fileName);
  return stringArrayToNeuralNetwork(extractedText);

  }

*/


/*
  NeuralNetwork stringArrayToNeuralNetwork(String extractedText[]) {
  //calculate number of layers and neurons in them
  int numberOfLayers_n = 0;
  int numberOfNeurons_n_Temporary[10];
  String stringNumber = "";
  for (int i = 0; i < extractedText[0].length(); i++) {
    char c = extractedText[0].charAt(i);
    if (c == ',' || c == ';') {
      numberOfNeurons_n_Temporary[numberOfLayers_n] = int(stringNumber.toFloat());
      numberOfLayers_n++;
      stringNumber = "";
    } else stringNumber += c;
  }
  //assign numbers of neurons
  int numberOfNeurons_n[numberOfLayers_n];
  for (int i = 0; i < numberOfLayers_n; i++) numberOfNeurons_n[i] = numberOfNeurons_n_Temporary[i];
  //create a neural network of this shape
  NeuralNetwork n;
  n.NeuralNetworkConstructor(numberOfNeurons_n, sizeof(numberOfNeurons_n) / sizeof(numberOfNeurons_n[0]));

  //find weights
  int layer = 0, i = 0, j = 0;
  stringNumber = "";
  boolean doneWithWeights = false, metAnOpenBracket = false;
  int line = 1;
  for (; line < sizeof(extractedText) / sizeof(extractedText[0]) && !doneWithWeights; line++) {
    for (int index = 0; index < extractedText[line].length() && !doneWithWeights; index++) {
      char c = extractedText[line].charAt(index);
      if (layer == n.numberOfLayers - 1) doneWithWeights = true;
      else if (extractedText[line].length() < 2) line++;
      else if (i >= n.weights[layer].rows) {
        i = 0;
        j = 0;
        layer++;
      }
      else if (c == '{') {
        metAnOpenBracket = true;
      }
      else if (c == '}') {
        n.weights[layer].data[i][j] = stringNumber.toFloat();
        stringNumber = "";
        i++;
        j = 0;
        line++;
        index = -1;
        metAnOpenBracket = false;
      }
      else if (c == ',') {
        n.weights[layer].data[i][j] = stringNumber.toFloat();
        stringNumber = "";
        j++;
      }
      else if (metAnOpenBracket &&
               ((int(c) >= int('0') && int(c) <= int('9')) || c == '-' || c == '.')) stringNumber += c;
    }
  }

  //find biases
  layer = 0;
  i = 0;
  stringNumber = "";
  line--;
  boolean doneWithBiases = false;
  metAnOpenBracket = false;
  for (; line < sizeof(extractedText) / sizeof(extractedText[0]) && !doneWithBiases; line++) {
    for (int index = 0; index < extractedText[line].length() && !doneWithBiases; index++) {
      char c = extractedText[line].charAt(index);
      if (layer == n.numberOfLayers - 1) doneWithBiases = true;
      else if (extractedText[line].length() < 2) line++;
      else if (i >= n.biases[layer].rows) {
        i = 0;
        layer++;
      }
      else if (c == '{') metAnOpenBracket = true;
      else if (c == '}') {
        n.biases[layer].data[i][0] = stringNumber.toFloat();
        stringNumber = "";
        i = 0;
        line++;
        index = 0;
        metAnOpenBracket = false;
        layer++;
      }
      else if (c == ',') {
        n.biases[layer].data[i][0] = stringNumber.toFloat();
        stringNumber = "";
        i++;
      }
      else if (metAnOpenBracket &&
               ((int(c) >= int('0') && int(c) <= int('9')) || c == '-' || c == '.')) stringNumber += c;
    }
  }
  return n;
  }


  NeuralNetwork longStringToNeuralNetwork(String text) {
  //calculate number of layers and neurons in them
  int numberOfLayers_n = 0;
  int numberOfNeurons_n_Temporary[10];
  String stringNumber = "";
  for (int i = 0; text.charAt(i) != '\n'; i++) {
    char c = text.charAt(i);
    if (c == ',' || c == ';') {
      numberOfNeurons_n_Temporary[numberOfLayers_n] = int(stringNumber.toFloat());
      numberOfLayers_n++;
      stringNumber = "";
    } else stringNumber += c;
  }

  int lines = 2 + 2 * (numberOfLayers_n - 1) + 2; //last +2 just in case
  for (int i = 1; i < numberOfLayers_n; i++) {
    lines += numberOfNeurons_n_Temporary[numberOfLayers_n] + 2;
  }
  String extractedTextArray[lines];
  for (int i = 0; i < sizeof(extractedTextArray) / sizeof(extractedTextArray[0]); i++) extractedTextArray[i] = "";
  int line = 0;
  for (int i = 0; i < text.length(); i++) {
    char c = text.charAt(i);
    if (c == '\n') line++;
    else extractedTextArray[line] += c;
  }
  return stringArrayToNeuralNetwork(extractedTextArray);
  }

*/
