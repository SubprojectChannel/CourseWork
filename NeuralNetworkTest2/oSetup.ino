NeuralNetwork brain;

void setup() {
  Serial.begin(9600);
  Serial.println("test1rewq");
  Serial.println("test1rewq");
  int myNN[] = {5, 3, 2};
  //brain = fileToNeuralNetwork("testSaving3.txt");
  Serial.println((sizeof(myNN)/sizeof(myNN[0]))*3);
  brain.NeuralNetworkConstructor(myNN, sizeof(myNN)/sizeof(myNN[0]));
  Serial.println("layers:" + String(brain.numberOfLayers));
  for(int i = 0 ; i < brain.numberOfLayers-1; i++) {
    brain.weights[i].setName("weights"+i);
    brain.biases[i].setName("biases"+i);
  }
  for(int i = 0 ; i < brain.numberOfLayers-1; i++) {
    Serial.println("loop through layers of weights");
  }
  Serial.println("test3");
  Serial.println(brain.printOut());
  Serial.println("done with printout");
}
