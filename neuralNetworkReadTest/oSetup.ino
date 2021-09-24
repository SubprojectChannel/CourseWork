Function sigmoid = new Function(0);
Function dSigmoid = new Function(1);

NeuralNetwork brain;

void setup() {
  int myNN[] = {5, 3, 2};
  //brain = fileToNeuralNetwork("testSaving3.txt");
  brain = new NeuralNetwork(myNN);
  for(int i = 0 ; i < brain.weights.length; i++) {
    brain.weights[i].setName("weights"+i);
    brain.biases[i].setName("biases"+i);
  }
  println(brain.printOut());
  saveNeuralNetworkToTxt(brain, "data/testSaving5");
  println("done with saving to txt");
}
