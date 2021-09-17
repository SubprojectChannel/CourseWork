//constants
float e = 2.71828;

//neuron graphics
float neuronDiameter = 0.05; //of height
float neuronHorisontalSpacing;
float neuronVerticalSpacing[];

Boolean training = false;

float input[];
float desiredOutput[] = {1, 0}; //10 if middle neuron is > 0.5 // 01 if middle neuron is < 0.5

Function sigmoid = new Function(0);
Function dSigmoid = new Function(1);

NeuralNetwork brain;
int myNeurons[] = {5, 3, 4, 20, 2};

void setup() {
  //size(1000, 500);
  fullScreen();
  
  //initialise neural network
  brain = new NeuralNetwork(myNeurons, 0.001);
  //brain = fileToNeuralNetwork("testSaving3.txt");
  
  //define variables which needed network defined
  neuronVerticalSpacing = new float[brain.numberOfLayers];
  input = new float[brain.numberOfNeurons[0]];
  
  //defining graphicals
  neuronDiameter *= height;
  for (int i = 0; i < brain.numberOfLayers; i++) {
    neuronVerticalSpacing[i] = (height - neuronDiameter*brain.numberOfNeurons[i])/(brain.numberOfNeurons[i]+1);
  }
  neuronHorisontalSpacing = (width - neuronDiameter*brain.numberOfLayers - 2*neuronDiameter)/(brain.numberOfLayers-1);
  
  //creating a matrix of training inputs and outputs
  textToInputOutput(loadStrings("trainingData10.txt"));
  
}

void draw() {
  background(0);
  if(training) {
    brain.fit(inputs, outputs, 1000);
  }
  else {
  }
  drawNetwork(brain);
  //drawWeightsValues(brain);
  //drawBiasesValues(brain);
}
