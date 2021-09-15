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

class Matrix {
  public:
    int rows, cols;
    float** data;
    String name = "matrix";

    void MatrixConstructor(int rows_, int cols_) {
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

    Matrix MatrixMultiply(Matrix A, Matrix B) {
      if (A.cols != B.rows) {
        Serial.println("Size Mismatch in MatrixMultiply(Matrix a)");
        return B;
      }
      float** a = A.data;
      float** b = B.data;
      Matrix newMatrix;
      newMatrix.MatrixConstructor(A.rows, B.cols);
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
      Matrix result;
      result.MatrixConstructor(a.rows, a.cols);
      for (int i = 0; i < a.rows; i++) for (int j = 0; j < a.cols; j++) result.data[i][j] = a.data[i][j] - b.data[i][j];
      return result;
    }

    Matrix MatrixTranspose(Matrix b) {
      Matrix newMatrix;
      newMatrix.MatrixConstructor(b.cols, b.rows);
      for (int i = 0; i < b.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
          newMatrix.data[j][i] = b.data[i][j];
        }
      }
      return newMatrix;
    }

    Matrix MatrixFromArray(float Array[]) {
      Matrix m;
      m.MatrixConstructor(sizeof(Array) / sizeof(Array[0]), 1);
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

    Matrix MatrixMap(Matrix M, Function f, float parameters[]) {
      Matrix m = M;
      for (int i = 0; i < m.rows; i++) for (int j = 0; j < m.cols; j++) m.data[i][j] = f.apply(M.data[i][j], parameters, i, j, f.index);
      return m;
    }
    Matrix MatrixMap(Matrix M, Function f, float a) {
      Matrix m = M;
      float parameters[] = {a};
      for (int i = 0; i < m.rows; i++) for (int j = 0; j < m.cols; j++) m.data[i][j] = f.apply(M.data[i][j], parameters, i, j, f.index);
      return m;
    }

    Matrix MatrixMap(Matrix M, Function f) {
      Matrix m = M;
      float parameters[] = {};
      for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) m.data[i][j] = f.apply(M.data[i][j], parameters, i, j, f.index);
      }
      return m;
    }

    String arrayToString(Matrix ArrayMatrix, String text, int oneDimentional) {
      float Array[ArrayMatrix.rows];
      for (int i = 0; i < ArrayMatrix.rows; i++) Array[i] = ArrayMatrix.data[i][0];
      text += "{";
      for (int i = 0; i < sizeof(Array) / sizeof(Array[0]); i++) {
        if (i < sizeof(Array) / sizeof(Array[0]) - 1) text += (String(Array[i]) + ",");
        else text += (String(Array[i]));
      }
      text += "}";
      return text;
    }

    String arrayToString(Matrix ArrayMatrix, String text) {
      float Array[ArrayMatrix.rows][ArrayMatrix.cols];
      for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) Array[i][j] = ArrayMatrix.data[i][j];
      text += "{\n";
      for (int i = 0; i < ArrayMatrix.rows; i++) {
        Matrix ArrayMatrix_i_row;
        ArrayMatrix_i_row.MatrixConstructor(ArrayMatrix.cols, 1);
         for (int j = 0; j < ArrayMatrix.cols; j++) ArrayMatrix_i_row.data[j][0] = ArrayMatrix.data[i][j];
        text = arrayToString(ArrayMatrix_i_row, text, 1);
        if (i < ArrayMatrix.rows - 1) text += ",\n";
        else {
        }
      }
      text += "}";
      return text;
    }
};

Matrix matrix;

class NeuralNetwork {
  public:
    float learning_rate = 0.1;
    int numberOfLayers;
    int* numberOfNeurons;
    Matrix* neurons;
    Matrix* weights;
    Matrix* biases;

    void NeuralNetworkConstructor(int NofNeurons[], int numberOfLayers_) {
      NeuralNetworkConstructor2(NofNeurons, numberOfLayers_, learning_rate);
    }

    void NeuralNetworkConstructor(int NofNeurons[], int numberOfLayers_, float lr) {
      NeuralNetworkConstructor2(NofNeurons, numberOfLayers_, lr);
    }

    void NeuralNetworkConstructor2(int NofNeurons[], int numberOfLayers_, float lr) {
      //define shape of the network
      numberOfLayers = numberOfLayers_;
      numberOfNeurons = NofNeurons;
      Serial.println("q " +  String(sizeof(NofNeurons) / sizeof(NofNeurons[0])));
      Serial.println("w " + String(numberOfNeurons[2]));
      Matrix neuronsArray[numberOfLayers];
      Matrix weightsArray[numberOfLayers - 1];
      Matrix biasesArray[numberOfLayers - 1];
      neurons = neuronsArray;
      weights = weightsArray;
      biases = biasesArray;
      //initialise neurons, weights, biases
      for (int i = 0; i < numberOfLayers - 1; i++) {
        weights[i].MatrixConstructor(numberOfNeurons[i + 1], numberOfNeurons[i]);
        biases[i].MatrixConstructor(numberOfNeurons[i + 1], 1);
        neurons[i].MatrixConstructor(numberOfNeurons[i], 1);

        weights[i].randomise(); //might wanna do this in the constructor
        biases[i].randomise();
      }
      neurons[numberOfLayers - 1].MatrixConstructor(numberOfNeurons[numberOfLayers - 1], 1); //last layer that didn't fit in the loop

      learning_rate = lr;
    }

    float* feedForward(float input_array[]) {
      neurons[0] = matrix.MatrixFromArray(input_array);

      for (int i = 0; i < numberOfLayers - 1; i++) {
        neurons[i + 1] = matrix.MatrixMultiply(weights[i], neurons[i]);
        neurons[i + 1].add(biases[i]);
        neurons[i + 1].map(sigmoid);
      }

      return matrix.MatrixToArray(neurons[numberOfLayers - 1]);
    }

    void train(float input_array[], float desiredOutputs_array[]) {
      //feed forward the input to calculate all neuron values
      feedForward(input_array);

      Matrix previous_errors = neurons[1];
      for (int i = numberOfLayers - 1; i > 0; i--) {
        //calculate errors
        Matrix errors; //sketchy, do I need to initialise it?
        if (i == numberOfLayers - 1) errors = matrix.MatrixSubtract(matrix.MatrixFromArray(desiredOutputs_array), neurons[i]);
        else errors = matrix.MatrixMultiply(matrix.MatrixTranspose(weights[i]), previous_errors);
        //calculate gradients
        Matrix gradients = matrix.MatrixMap(neurons[i], dSigmoid);
        gradients.multiply(errors);
        gradients.multiply(learning_rate);
        //adjust biases
        biases[i - 1].add(gradients);
        //calculate deltas for weights
        Matrix weights_deltas = matrix.MatrixMultiply(gradients, matrix.MatrixTranspose(neurons[i - 1]));
        //adjust weights
        weights[i - 1].add(weights_deltas);

        previous_errors = errors;
      }
    }

    void fit(Matrix inputsM, Matrix outputsM, int epochs) {
      float inputs[inputsM.rows][inputsM.cols], outputs[outputsM.rows][outputsM.cols];
      for (int i = 0; i < inputsM.rows; i++)for (int j = 0; j < inputsM.cols; j++) inputs[i][j] = inputsM.data[i][j];
      for (int i = 0; i < outputsM.rows; i++)for (int j = 0; j < outputsM.cols; j++) outputs[i][j] = outputsM.data[i][j];
      for (int i = 0; i < epochs; i++) {
        int sampleN = int(random(0, 1) * sizeof(inputs) / sizeof(inputs[0]));
        train(inputs[sampleN], outputs[sampleN]);
      }
    }

    String printOut() {
      String text = "";
      //print shape
      for (int i = 0; i < numberOfLayers; i++) {
        text += String(numberOfNeurons[i]);
        if (i < numberOfLayers - 1) text += ", ";
      }
      text += ";\n\n";

      //print weights
      for (int i = 0; i < numberOfLayers - 1; i++) {
        text += matrix.arrayToString(weights[i], "float " + weights[i].name + "[][] = ") + ";\n\n";
      }
      //print biases
      for (int i = 0; i < numberOfLayers - 1; i++) {
        text += matrix.arrayToString(biases[i], "float " + biases[i].name + "[] = ", 1) + ";\n\n";
      }
      return text;
    }

    void assignRandomNetwork() {
      for (int i = 0; i < numberOfLayers - 1; i++) {
        weights[i].randomise();
        biases[i].randomise();
      }
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
  n.NeuralNetworkConstructor(numberOfNeurons_n, sizeof(numberOfNeurons_n)/ sizeof(numberOfNeurons_n[0]));

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
