void setup() {
  Serial.begin(9600);
  delay(2000);
  randomSeed(analogRead(0));
  Serial.println(freeMemory());

  int myNN[] = {2, 4, 1};
  NeuralNetwork brain(myNN, sizeof(myNN) / sizeof(myNN[0]));
  brain.learning_rate = 0.2;

  float inputs[4][2] = {
    {0, 0},
    {1, 0},
    {0, 1},
    {1, 1}
  };

  float outputs[4][1] = {
    {0}, {1}, {1}, {0}
  };

  //convert inputs and outputs to matrix objects
  Matrix inputsM(sizeof(inputs) / sizeof(inputs[0]), sizeof(inputs[0]) / sizeof(inputs[0][0]), "inputsM");
  Matrix outputsM(sizeof(outputs) / sizeof(outputs[0]), sizeof(outputs[0]) / sizeof(outputs[0][0]), "outputsM");
  for (int i = 0; i < inputsM.rows; i++) {
    for (int j = 0; j < inputsM.cols; j++) {
      inputsM.data[i][j] = inputs[i][j];
    }
    for (int j = 0; j < outputsM.cols; j++) {
      outputsM.data[i][j] = outputs[i][j];
    }
  }
  inputsM.printOut();
  outputsM.printOut();

  brain.fit(&inputsM, &outputsM, 10000, 1);

  mem();
  for (int i = 0; i < 4; i++) {
    Array input_Array(inputsM.data[i], 2);
    Array output_Array(outputsM.data[i], 1);
    input_Array.setName("input_Array");
    output_Array.setName("output_Array");
    //input_Array.printOut();
    //output_Array.printOut();
    Array testFeed(input_Array);
    brain.forwardPropagation(&testFeed);
    Matrix testOutput(&brain.neurons[brain.numberOfLayers - 1], "testOutput");
    testOutput.printOut();
  }
  mem();
  Serial.print("///////////////////////////////////////////////////////////////////////");
}

void loop() {
}
