/*

NeuralNetwork fileToNeuralNetwork(String fileName) {
  //extract text
  String extractedText[] = loadStrings(fileName);
  return stringArrayToNeuralNetwork(extractedText);
  
}
NeuralNetwork stringArrayToNeuralNetwork(String extractedText[]) {
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


NeuralNetwork longStringToNeuralNetwork(String text) {
  //calculate number of layers and neurons in them
  int numberOfLayers_n = 0;
  int numberOfNeurons_n_Temporary[] = new int[10];
  String stringNumber = "";
  for (int i = 0; text.charAt(i) != '\n'; i++) {
    char c = text.charAt(i);
    if (c == ',' || c == ';') {
      numberOfNeurons_n_Temporary[numberOfLayers_n] = int(float(stringNumber));
      numberOfLayers_n++;
      stringNumber = "";
    } else stringNumber += c;
  }
  
  int lines = 2 + 2*(numberOfLayers_n-1) + 2; //last +2 just in case
  for (int i = 1; i < numberOfLayers_n; i++) {
    lines += numberOfNeurons_n_Temporary[numberOfLayers_n]+2;
  }
  String extractedTextArray[] = new String[lines];
  for(int i = 0; i < extractedTextArray.length; i++) extractedTextArray[i] = "";
  int line = 0;
  for(int i = 0; i < text.length(); i++) {
    char c = text.charAt(i);
    if(c == '\n') line++;
    else extractedTextArray[line] += c;
  }
  return stringArrayToNeuralNetwork(extractedTextArray);
}
*/
