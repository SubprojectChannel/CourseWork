class NeuralNetwork {
  float learning_rate = 0.1;
  int numberOfNeurons[], numberOfLayers;
  Matrix neurons[], weights[], biases[];
  
  NeuralNetwork(int[] NofNeurons) {
    NeuralNetworkConstructor(NofNeurons, learning_rate);
  }
  
  NeuralNetwork(int[] NofNeurons, float lr) {
    NeuralNetworkConstructor(NofNeurons, lr);
  }
  
  void NeuralNetworkConstructor(int[] NofNeurons, float lr) {
    //define shape of the network
    numberOfNeurons = NofNeurons;
    numberOfLayers = numberOfNeurons.length;
    //define neurons, weights, biases
    neurons = new Matrix[numberOfLayers];
    weights = new Matrix[numberOfLayers-1];
    biases = new Matrix[numberOfLayers-1];
    //initialise neurons, weights, biases
    for(int i = 0; i < numberOfLayers - 1; i++) {
      weights[i] = new Matrix(numberOfNeurons[i+1], numberOfNeurons[i]);
      biases[i] = new Matrix(numberOfNeurons[i+1], 1);
      neurons[i] = new Matrix(numberOfNeurons[i], 1);
      
      weights[i].randomise(); //might wanna do this in the constructor
      biases[i].randomise();
    }
    println(numberOfNeurons[numberOfLayers-1]);
    neurons[numberOfLayers-1] = new Matrix(numberOfNeurons[numberOfLayers-1], 1); //last layer that didn't fit in the loop
    learning_rate = lr;
  }
  
  float[] feedForward(float[] input_array) {
    neurons[0] = MatrixFromArray(input_array);
    
    for(int i = 0; i < numberOfLayers - 1; i++) {
      neurons[i+1] = MatrixMultiply(weights[i], neurons[i]);
      neurons[i+1].add(biases[i]);
      neurons[i+1].map(sigmoid);
    }
    
    return MatrixToArray(neurons[numberOfLayers-1]);
  }
  
  void train(float[] input_array, float[] desiredOutputs_array) {
    //feed forward the input to calculate all neuron values
    feedForward(input_array);
    
    Matrix previous_errors = neurons[1];
    for(int i = numberOfLayers - 1; i > 0; i--) {
      //calculate errors
      Matrix errors;
      if(i == numberOfLayers - 1) errors = MatrixSubtract(MatrixFromArray(desiredOutputs_array), neurons[i]);
      else errors = MatrixMultiply(MatrixTranspose(weights[i]), previous_errors);
      //calculate gradients
      Matrix gradients = map(neurons[i], dSigmoid);
      gradients.multiply(errors);
      gradients.multiply(learning_rate);
      //adjust biases
      biases[i-1].add(gradients);
      //calculate deltas for weights
      Matrix weights_deltas = MatrixMultiply(gradients, MatrixTranspose(neurons[i-1]));
      //adjust weights
      weights[i-1].add(weights_deltas);
    
      previous_errors = errors;
    }
  }
  
  void fit(float[][] inputs, float[][] outputs, int epochs) {
    for(int i = 0; i < epochs; i++) {
      int sampleN = int(random(0, 1)*inputs.length);
      this.train(inputs[sampleN], outputs[sampleN]);
    }
  }
  
  String printOut() {
    String text = "";
    //print shape
    for(int i = 0; i < numberOfLayers; i++) {
      text += str(numberOfNeurons[i]);
      if(i < numberOfLayers-1) text += ", ";
    }
    text += ";\n\n";
    
    //print weights
    for(int i = 0; i < weights.length; i++) {
      text += arrayToString(weights[i].data, "float " + weights[i].name + "[][] = ")+";\n\n";
    }
    //print biases
    for(int i = 0; i < biases.length; i++) {
      text += arrayToString(MatrixToArray(biases[i]), "float " + biases[i].name + "[] = ")+";\n\n";
    }
    return text;
  }
  
  String printOutWithPointers() {
    String text = "";
    //print shape
    for(int i = 0; i < numberOfLayers; i++) {
      text += str(numberOfNeurons[i]);
      if(i < numberOfLayers-1) text += ", ";
    }
    text += ";\n\n";
    
    //print weights
    for(int i = 0; i < weights.length; i++) {
      text += arrayToString(weights[i].data, "float " + weights[i].name + "[][] = ")+";\n\n";
    }
    //print biases
    for(int i = 0; i < biases.length; i++) {
      text += arrayToString(MatrixToArray(biases[i]), "float " + biases[i].name + "[] = ")+";\n\n";
    }
    text += definePointersArduino(weights, "weightPointer");
    text += definePointersArduino(biases, "biasPointer");
    return text;
  }
  
  void assignRandomNetwork() {
    for(int i = 0; i < numberOfLayers-1; i++) {
      weights[i].randomise();
      biases[i].randomise();
    }
  }
}
