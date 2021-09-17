float inputs[][];
float outputs[][];

//String myExtractedText[] = loadStrings("trainingData9.txt");
//textToInputOutput(myExtractedText);
//printInputOutput();
  
void textToInputOutput(String extractedText[]) {
  int numberOfNeuronsFirst;
  int numberOfNeuronsLast;
  int numberOfTrainingSamples;

  //calculating the number of training samples
  numberOfTrainingSamples = (extractedText.length-5)/2;
  
  //calculating the number of neurons in the first layer
  char c = extractedText[1].charAt(0);
  numberOfNeuronsFirst = 1;
  for (int i = 0; c != '}'; i++) {
    c = extractedText[1].charAt(i);
    if (c == ',') numberOfNeuronsFirst++;
  }

  //calculating the number of neurons in the last layer
  int whereOutputStarts = 3 + numberOfTrainingSamples;
  c = extractedText[whereOutputStarts+1].charAt(0); //;.,lkjhgufytdfghujiko
  numberOfNeuronsLast = 1;
  for (int i = 0; c != '}'; i++) {
    c = extractedText[whereOutputStarts+1].charAt(i);
    if (c == ',') numberOfNeuronsLast++;
  }
  
  
  if(numberOfNeuronsFirst != brain.numberOfNeurons[0] || numberOfNeuronsLast != brain.numberOfNeurons[brain.numberOfLayers-1]) {
    println("Training data shape and network shape mismatch");
    println("Ti: " + numberOfNeuronsFirst + " != Ni: " + brain.numberOfNeurons[0] + "  or  To: " + numberOfNeuronsLast + " != No: " + brain.numberOfNeurons[brain.numberOfLayers-1]);
    return;
  }
  inputs = new float[numberOfTrainingSamples][numberOfNeuronsFirst];
  outputs = new float[numberOfTrainingSamples][numberOfNeuronsLast];

  //extracting inputs
  for (int i = 0; i < inputs.length; i++) {
    int whereNumberStarts = 1;
    for (int j = 0; j < inputs[i].length; j++) {
      char currentChar = char(extractedText[1+i].charAt(whereNumberStarts));
      String myNumberString = "";
      for (; currentChar != ',' && currentChar != '}'; whereNumberStarts++, currentChar = extractedText[1+i].charAt(whereNumberStarts)) {
        myNumberString += currentChar;
      }
      whereNumberStarts+=2;
      inputs[i][j] = float(myNumberString);
    }
  }

  //extracting outputs
  for (int i = 0; i < outputs.length; i++) {
    int whereNumberStarts = 1;
    for (int j = 0; j < outputs[i].length; j++) {
      char currentChar = char(extractedText[i+whereOutputStarts+1].charAt(whereNumberStarts));
      String myNumberString = "";
      for (; currentChar != ',' && currentChar != '}'; whereNumberStarts++, currentChar = char(extractedText[i+whereOutputStarts+1].charAt(whereNumberStarts))) {
        myNumberString += currentChar;
      }
      whereNumberStarts+=2;
      outputs[i][j] = float(myNumberString);
    }
  }
}


void printInputOutput() {
  int numberOfTrainingSamples = inputs.length;

  //printing input
   println("float inputs[][] = {");
   for(int i = 0; i < inputs.length; i++) {
   print("{");
   for(int j = 0; j < inputs[0].length; j++) {
   if(j < inputs[0].length - 1) print(str(inputs[i][j]) + ", ");
   else print(str(inputs[i][j]));
   }
   if(i < numberOfTrainingSamples - 1) println("},");
   else println("}");
   }
   println("};");
   
   //printing outputs
   println("\nfloat outputs[][] = {");
   for(int i = 0; i < outputs.length; i++) {
   print("{");
   for(int j = 0; j < outputs[0].length; j++) {
   if(j < outputs[0].length - 1) print(str(outputs[i][j]) + ", ");
   else print(str(outputs[i][j]));
   }
   if(i < numberOfTrainingSamples - 1) println("},");
   else println("}");
   }
   println("};");
   
}


float[] inputIteration(int index) {
  float i[] = {1};
  return i;
  //return inputs[index];
}

float[] outputIteration(int index) {
  float o[] = {1, 0};
  return o;
  //return outputs[index];
}
/*
void getInputOutput(int i) {
  if(training) {
    if(i < 0)  print("assign input index less than 0 :", i);
    if(i >= inputs.length)  print("assign input index bigger than inputs.length (", inputs.length, ") :", i);
    i = constrain(i, 0, inputs.length-1);
    input = inputIteration(i);
    desiredOutput = outputIteration(i);
  }
}
*/
