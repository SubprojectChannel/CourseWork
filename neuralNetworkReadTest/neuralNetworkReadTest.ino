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


class Matrix {
  public:
    int rows, cols;
    float** data;
    String name = "matrix";

    Matrix(int rows_, int cols_) {
      rows = rows_;
      cols = cols_;
      float myData[rows][cols];
      float* dataRows[rows];
      for (int i = 0; i < rows; i++) dataRows[i] = myData[i];
      data = dataRows;
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = 0;
      //randomise();
    }

    void setName(String name_) {
      name = name_;
    }
    void randomise() {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = random(-1, 1);
    }

    void multiply(float scaler) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] *= scaler;
    }

    void multiply(Matrix a) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] *= a.data[i][j];
    }

    void add(float add) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] += add;
    }

    void add(Matrix a) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] += a.data[i][j];
    }


    void map(Function f, float parameters[]) {
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = f.apply(data[i][j], parameters, i, j, f.index);
    }
    void map(Function f, float a) {
      float parameters[] = {a};
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = f.apply(data[i][j], parameters, i, j, f.index);
    }
    void map(Function f) {
      float parameters[] = {};
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = f.apply(data[i][j], parameters, i, j, f.index);
    }
    /*
        void transpose() {
          int oldRows = rows;
          int oldCols = cols;
          rows = oldCols;
          cols = oldRows;
          float newData[oldCols][oldRows];
          for (int i = 0; i < oldRows; i++) {
            for (int j = 0; j < oldCols; j++) {
              newData[j][i] = data[i][j];
            }
          }
          data = newData;
        }
    */
    /*
        void printOut(String name) {
          Serial.println("Matrix", name, "= {");
          for (int i = 0; i < data.length; i++) {
            SErial.print("{");
            for (int j = 0; j < data[0].length; j++) {
              if (j < data[0].length - 1) print(str(data[i][j]) + ", ");
              else print(str(data[i][j]));
            }
            if (i < data.length - 1) println("},");
            else println("}");
          }
          println("};");
        }
    */
};




Matrix MatrixMultiply(Matrix A, Matrix B) {
  if (A.cols != B.rows) {
    Serial.println("Size Mismatch in MatrixMultiply(Matrix a)");
    return B;
  }
  float** a = A.data;
  float** b = B.data;
  Matrix newMatrix(A.rows, B.cols);
  for (int i = 0; i < newMatrix.rows; i++) {
    for (int j = 0; j < newMatrix.cols; j++) {
      float sum = 0;
      for (int k = 0; k < A.cols; k++) {
        sum += a[i][k] * b[k][j];
      }
      newMatrix.data[i][j] = sum;
    }
  }
  return newMatrix;
}

Matrix MatrixSubtract(Matrix a, Matrix b) {
  Matrix result(a.rows, a.cols);
  for (int i = 0; i < a.rows; i++) for (int j = 0; j < a.cols; j++) result.data[i][j] = a.data[i][j] - b.data[i][j];
  return result;
}

Matrix MatrixTranspose(Matrix b) {
  Matrix newMatrix(b.cols, b.rows);
  for (int i = 0; i < b.rows; i++) {
    for (int j = 0; j < b.cols; j++) {
      newMatrix.data[j][i] = b.data[i][j];
    }
  }
  return newMatrix;
}

Matrix MatrixFromArray(float Array[]) {
  Matrix m(sizeof(Array) / sizeof(Array[0]), 1);
  for (int i = 0; i < m.rows; i++) {
    m.data[i][0] = Array[i];
  }
  return m;
}

float* MatrixToArray(Matrix m) {
  float a[m.rows * m.cols];
  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < m.cols; j++) {
      //println(i*m.rows+j, i, j);
      a[i * m.cols + j] = m.data[i][j];
    }
  }
  return a;
}

Matrix map(Matrix M, Function f, float parameters[]) {
  Matrix m = M;
  for (int i = 0; i < m.rows; i++) for (int j = 0; j < m.cols; j++) m.data[i][j] = f.apply(M.data[i][j], parameters, i, j, f.index);
  return m;
}
Matrix map(Matrix M, Function f, float a) {
  Matrix m = M;
  float parameters[] = {a};
  for (int i = 0; i < m.rows; i++) for (int j = 0; j < m.cols; j++) m.data[i][j] = f.apply(M.data[i][j], parameters, i, j, f.index);
  return m;
}

Matrix map(Matrix M, Function f) {
  Matrix m = M;
  float parameters[] = {};
  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < m.cols; j++) m.data[i][j] = f.apply(M.data[i][j], parameters, i, j, f.index);
  }
  return m;
}

class NeuralNetwork {
  public:
    float learning_rate = 0.1;
    int numberOfLayers;
    int* numberOfNeurons;
    Matrix** neurons, weights, biases;

    NeuralNetwork(int NofNeurons[]) {
      NeuralNetworkConstructor(NofNeurons, learning_rate);
    }

    NeuralNetwork(int NofNeurons[], float lr) {
      NeuralNetworkConstructor(NofNeurons, lr);
    }

    void NeuralNetworkConstructor(int NofNeurons[], float lr) {
      //define shape of the network
      numberOfNeurons = NofNeurons;
      numberOfLayers = numberOfNeurons.length;
      //define neurons, weights, biases
      Matrix* neuronsArray[numberOfLayers];
      Matrix* weightsArray[numberOfLayers - 1];
      Matrix* biasesArray[numberOfLayers - 1];
      //initialise neurons, weights, biases
      for (int i = 0; i < numberOfLayers - 1; i++) {
        weightsArray[i] = (numberOfNeurons[i + 1], numberOfNeurons[i]);
        biasesArray[i] = (numberOfNeurons[i + 1], 1);
        neuronsArray[i] = (numberOfNeurons[i], 1);

        weightsArray[i].randomise(); //might wanna do this in the constructor
        biasesArray[i].randomise();
      }
      neurons[numberOfLayers - 1] = new Matrix(numberOfNeurons[numberOfLayers - 1], 1); //last layer that didn't fit in the loop

      weights = weightsArray;
      biases = biasesArray;
      neurons = neuronsArray;

      learning_rate = lr;
    }

    float* feedForward(float input_array[]) {
      neurons[0] = MatrixFromArray(input_array);

      for (int i = 0; i < numberOfLayers - 1; i++) {
        neurons[i + 1] = MatrixMultiply(weights[i], neurons[i]);
        neurons[i + 1].add(biases[i]);
        neurons[i + 1].map(sigmoid);
      }

      return MatrixToArray(neurons[numberOfLayers - 1]);
    }

    void train(float input_array[], float desiredOutputs_array[]) {
      //feed forward the input to calculate all neuron values
      feedForward(input_array);

      Matrix previous_errors = neurons[1];
      for (int i = numberOfLayers - 1; i > 0; i--) {
        //calculate errors
        Matrix errors(1, 1); //sketchy, does this 1,1 affect stuff?
        if (i == numberOfLayers - 1) errors = MatrixSubtract(MatrixFromArray(desiredOutputs_array), neurons[i]);
        else errors = MatrixMultiply(MatrixTranspose(weights[i]), previous_errors);
        //calculate gradients
        Matrix gradients = map(neurons[i], dSigmoid);
        gradients.multiply(errors);
        gradients.multiply(learning_rate);
        //adjust biases
        biases[i - 1].add(gradients);
        //calculate deltas for weights
        Matrix weights_deltas = MatrixMultiply(gradients, MatrixTranspose(neurons[i - 1]));
        //adjust weights
        weights[i - 1].add(weights_deltas);

        previous_errors = errors;
      }
    }

    void fit(Matrix inputsM, Matrix outputsM, int epochs) {
      float inputs[][inputsM.cols] = MatrixToArray(inputsM);
      float outputs[][outputsM.cols] = MatrixToArray(outputsM);
      for (int i = 0; i < epochs; i++) {
        int sampleN = int(random(0, 1) * inputs.length);
        this.train(inputs[sampleN], outputs[sampleN]);
      }
    }

    String printOut() {
      String text = "";
      //print shape
      for (int i = 0; i < numberOfLayers; i++) {
        text += str(numberOfNeurons[i]);
        if (i < numberOfLayers - 1) text += ", ";
      }
      text += ";\n\n";

      //print weights
      for (int i = 0; i < weights.length; i++) {
        text += arrayToString(weights[i].data, "float " + weights[i].name + "[][] = ") + ";\n\n";
      }
      //print biases
      for (int i = 0; i < biases.length; i++) {
        text += arrayToString(MatrixToArray(biases[i]), "float " + biases[i].name + "[] = ") + ";\n\n";
      }
      return text;
    }

    String printOutWithPointers() {
      String text = "";
      //print shape
      for (int i = 0; i < numberOfLayers; i++) {
        text += str(numberOfNeurons[i]);
        if (i < numberOfLayers - 1) text += ", ";
      }
      text += ";\n\n";

      //print weights
      for (int i = 0; i < weights.length; i++) {
        text += arrayToString(weights[i].data, "float " + weights[i].name + "[][" + weights[i].data[0].length + "] = ") + ";\n\n";
      }
      //print biases
      for (int i = 0; i < biases.length; i++) {
        text += arrayToString(MatrixToArray(biases[i]), "float " + biases[i].name + "[] = ") + ";\n\n";
      }
      text += initialisePointers(weights, "weightPointer");
      text += initialisePointers(biases, "biasPointer");

      text += assignPointers(weights, "weightPointer");
      text += assignPointers(biases, "biasPointer");
      return text;
    }

    void assignRandomNetwork() {
      for (int i = 0; i < numberOfLayers - 1; i++) {
        weights[i].randomise();
        biases[i].randomise();
      }
    }
};
