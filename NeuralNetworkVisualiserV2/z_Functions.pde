String arrayToString(float[] array, String text) {
  text += "{";
  for (int i = 0; i < array.length; i++) {
    if (i < array.length-1) text += (str(array[i]) + ",");
    else text += (str(array[i]));
  }
  text += "}";
  return text;
}

String arrayToString(float[][] array, String text) {
  text += "{\n";
  for (int i = 0; i < array.length; i++) {
    text = arrayToString(array[i], text);
    if (i < array.length-1) text += ",\n";
    else {
    }
  }
  text += "}";
  return text;
}

String initializePointers(Matrix[] a, String pointerName) {
  String text = "";
  text += "char* " + pointerName + "[] = new char*[" + a.length + "];\n";
  return text;
}

String assignPointers(Matrix[] a, String pointerName) {
  String text = "";
  for (int i = 0; i < a.length; i++) {
    text += pointerName + "[" + i + "] = " + a[i].name + ";\n";
  }
  return text;
}

String definePointersArduino(Matrix[] a, String pointerName) {
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
  //calculate number of layers and neurons in them
  int numberOfLayers_n = 0;
  int numberOfNeurons_n_Temporary[] = new int[10];
  String stringNumber = "";
  for (int i = 0; i < extractedText[0].length(); i++) {
    char c = extractedText[0].charAt(i);
    if (c == ',' || c == ';') {
      numberOfNeurons_n_Temporary[numberOfLayers_n] = int(float(stringNumber));
      numberOfLayers_n++;
      stringNumber = "";
    } else stringNumber += c;
  }
  //assign numbers of neurons
  println(numberOfLayers_n);
  int numberOfNeurons_n[] = new int[numberOfLayers_n];
  for (int i = 0; i < numberOfLayers_n; i++) numberOfNeurons_n[i] = numberOfNeurons_n_Temporary[i];
  //create a neural network of this shape
  NeuralNetwork n = new NeuralNetwork(numberOfNeurons_n);
  
  //find weights
  int layer = 0, i = 0, j = 0;
  stringNumber = "";
  boolean doneWithWeights = false, metAnOpenBracket = false;
  int line = 1;
  for(; line < extractedText.length && !doneWithWeights; line++) {
    for(int index = 0; index < extractedText[line].length() && !doneWithWeights; index++) {
      char c = extractedText[line].charAt(index);
      if(layer == n.weights.length) doneWithWeights = true;
      else if(extractedText[line].length() < 2) line++;
      else if(i >= n.weights[layer].data.length) {
        i = 0;
        j = 0;
        layer++;
      }
      else if(c == '{') {
        metAnOpenBracket = true;
      }
      else if(c == '}') {
        n.weights[layer].data[i][j] = float(stringNumber);
        stringNumber = "";
        i++;
        j = 0;
        line++;
        index = -1;
        metAnOpenBracket = false;
      }
      else if(c == ',') {
        n.weights[layer].data[i][j] = float(stringNumber);
        stringNumber = "";
        j++;
      }
      else if(metAnOpenBracket && 
      ((int(c) >= int('0') && int(c) <= int('9')) || c == '-' || c == '.')) stringNumber += c;    }
  }
  
  //find biases
  layer = 0;
  i = 0;
  stringNumber = "";
  line--;
  boolean doneWithBiases = false;
  metAnOpenBracket = false;
  for(; line < extractedText.length && !doneWithBiases; line++) {
    for(int index = 0; index < extractedText[line].length() && !doneWithBiases; index++) {
      char c = extractedText[line].charAt(index);
      if(layer == n.biases.length) doneWithBiases = true;
      else if(extractedText[line].length() < 2) line++;
      else if(i >= n.biases[layer].data.length) {
        i = 0;
        layer++;
      }
      else if(c == '{') metAnOpenBracket = true;
      else if(c == '}') {
        n.biases[layer].data[i][0] = float(stringNumber);
        stringNumber = "";
        i = 0;
        line++;
        index = 0;
        metAnOpenBracket = false;
        layer++;
      }
      else if(c == ',') {
        n.biases[layer].data[i][0] = float(stringNumber);
        stringNumber = "";
        i++;
      }
      else if(metAnOpenBracket && 
      ((int(c) >= int('0') && int(c) <= int('9')) || c == '-' || c == '.')) stringNumber += c;
    }
  }
  return n;
}

void saveNeuralNetworkToTxt(NeuralNetwork n, String name) {
  PrintWriter neuralNetworkFile = createWriter(name + ".txt"); 
  neuralNetworkFile.println(n.printOut());
  neuralNetworkFile.flush();  // Writes the remaining data to the file
  neuralNetworkFile.close();  // Finishes the file
}

float average(float a[]) {
  float sum = 0;
  for(int i = 0; i < a.length; i++) sum += a[i];
  return sum/a.length;
}

float averageButLastOne(float a[]) {
  float sum = 0;
  for(int i = 0; i < a.length-1; i++) sum += a[i];
  return sum/a.length;
}

float maxOfArray(float a[]) {
  float maximum = a[0];
  for (int i = 1; i < a.length; i++) {
    if(a[i] > maximum) maximum = a[i];
  }
  return maximum;
}

float minOfArray(float a[]) {
  float minimum = a[0];
  for (int i = 1; i < a.length; i++) {
    if(a[i] < minimum) minimum = a[i];
  }
  return minimum;
}

float squishification(float x) {
  return 1.0/(1.0+pow(e, -x));
}

void feedForwardRandomInput() {
  generateInputOutput();
  brain.feedForward(input);
}

void generateInputOutput() {
  
  for(int i = 0; i < input.length; i++) {
      input[i] = random(0, 1);
    }
    input[input.length/2] = int(input[input.length/2] >= 0.5);
    desiredOutput[0] = int(input[input.length/2] >= 0.5);
    desiredOutput[1] = int(input[input.length/2] < 0.5);
  
}

void keyPressed() {
  if(key == 'r') brain.assignRandomNetwork();
  if(key == ' ') feedForwardRandomInput();
  else if(key == 't') {
    training = !training;
  }
}

void keyReleased() {
  //training = true;
}
