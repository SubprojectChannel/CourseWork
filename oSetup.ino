
float inputs[][5] = {
  {0.21386111, 0.94638675, 0.1548028, 0.20960897, 0.044149816},
  {0.7477825, 0.5254611, 0.7469225, 0.9514254, 0.59847575},
  {0.021902263, 0.32995826, 0.45149916, 0.7496793, 0.3465472},
  {0.6198675, 0.6255245, 0.5761538, 0.8088562, 0.941487},
  {0.21515805, 0.31365532, 0.8744001, 0.40784645, 0.9485363},
  {0.6182955, 0.42888337, 0.1595555, 0.55932415, 0.75027645},
  {0.17368764, 0.7824764, 0.1501233, 0.8570599, 0.3336358}
};

float outputs[][2] = {
  {0.0, 1.0},
  {1.0, 0.0},
  {0.0, 1.0},
  {1.0, 0.0},
  {1.0, 0.0},
  {0.0, 1.0},
  {0.0, 1.0}
};

/*
  float inputs[4][2] = {
    {0, 0},
    {1, 0},
    {0, 1},
    {1, 1}
  };

  float outputs[4][2] = {
    {0, 0}, {1, 1}, {1, 1}, {0, 0}
  };
*/

int myNN[] = {sizeof(inputs[0]) / sizeof(inputs[0][0]), 6, sizeof(outputs[0]) / sizeof(outputs[0][0])};
NeuralNetwork brain(myNN, sizeof(myNN) / sizeof(myNN[0]));

Matrix inputsM(sizeof(inputs) / sizeof(inputs[0]), sizeof(inputs[0]) / sizeof(inputs[0][0]), "inputsM");
Matrix outputsM(sizeof(outputs) / sizeof(outputs[0]), sizeof(outputs[0]) / sizeof(outputs[0][0]), "outputsM");

void setup() {
  Serial.begin(9600);

  for (int i = 8; i < 13; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for (int i = 6; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  //neural stuff
  delay(2000);
  randomSeed(analogRead(0));
  Serial.println(freeMemory());

  brain.learning_rate = 0.2;

  //convert inputs and outputs to matrix objects
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
  //
  //  brain.fit(&inputsM, &outputsM, 500, 0.05, 10);
  /*
    mem();
    for (int i = 0; i < 4; i++) {
      int sampleN = int(myRandom(0, inputsM.rows, 1));
      Array input_Array(inputsM.data[sampleN], inputsM.cols);
      Array output_Array(outputsM.data[sampleN], outputsM.cols);
      input_Array.setName("input_Array");
      output_Array.setName("output_Array");
      input_Array.printOut();
      //output_Array.printOut();
      Array testFeed(input_Array);
      brain.forwardPropagation(&testFeed);
      Matrix testOutput(&brain.neurons[brain.numberOfLayers - 1], "testOutput");
      testOutput.printOut();
      Serial.println("\n\n");
    }
  */
  //  mem();
  //  Serial.print("///////////////////////////////////////////////////////////////////////");
  //  inputsM.remove();
  //  outputsM.remove();

}

boolean training = false;
long startedPressingTime = 0;
boolean lastState = false;

void loop() {
  boolean nowState = !digitalRead(8);
  if (nowState && !lastState) startedPressingTime = millis();
  if (lastState && !nowState && millis() - startedPressingTime > 3000) {
    //lastTimeTrainingNotPressed = millis();
    training = !training;
  }
  lastState = nowState;

  if (training) {
    brain.fit(&inputsM, &outputsM, 30, 10);
    digitalWrite(7, int(millis())/100 % 2 == 0);
  }
  else {
    Array input_Array(5);
    for (int i = 8; i < 13; i++) {
      input_Array.data[i - 8] = int(!digitalRead(i));
    }
    Array testFeed(input_Array);
    brain.forwardPropagation(&testFeed);
    Matrix testOutput(&brain.neurons[brain.numberOfLayers - 1], "testOutput");
    analogWrite(6, 255 * testOutput.data[0][0]);
    analogWrite(7, 255 * testOutput.data[1][0]);
    input_Array.printOut();
    testOutput.printOut();
    Serial.println("\n\n");

    input_Array.remove();
    testFeed.remove();
    testOutput.remove();
    delay(100);
  }
}
