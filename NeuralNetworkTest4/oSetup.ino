
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
/*
  void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  mem();

  Matrix a(3, 1, "a"), b(3, 3, "b"), c(3, 3, "c");

  //Serial.println(freeMemory() + sizeof(a.name) + sizeof(b.name) + sizeof(c.name));
  mem2();
  a.randomise(0, 5, 1);
  b.randomise(0, 5, 1);
  mem2();
  a.printOut();
  b.printOut();
  mem2();

  Array d(a.data, a.rows, a.cols, "d");
  Array dd(a.data, a.rows, a.cols, "d");
  d.printOut();
  a.map(&b, &multiply, 2);
  a.printOut();
  mem2();
  a.map(&b, &multiply, 2);
  a.printOut();
  mem2();
  a.map(&b, &multiply, 2);
  a.printOut();
  mem2();
  a.map(&b, &multiply, 2);
  a.printOut();
  mem2();
  if(true) {
    Matrix r(4, 4);
    mem2();
    r.remove();
  }
  mem2();
  }


*/
/*
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
  Matrix inputsM(sizeof(inputs)/sizeof(inputs[0]), sizeof(inputs[0])/sizeof(inputs[0][0]), "inputsM");
  Matrix outputsM(sizeof(outputs)/sizeof(outputs[0]), sizeof(outputs[0])/sizeof(outputs[0][0]), "outputsM");
*/
void setup() {
  Serial.begin(9600);
  delay(2000);
  randomSeed(analogRead(0));
  Serial.println(freeMemory());

  int myNN[] = {2, 4, 1};
  NeuralNetwork brain(myNN, sizeof(myNN) / sizeof(myNN[0]));
  brain.learning_rate = 2;

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

  //Array input_Array(inputsM.data[1], 2);
  //Array output_Array(outputsM.data[1], 1);

  //input_Array.setName("input_Array");
  //output_Array.setName("output_Array");

  //input_Array.printOut();
  //output_Array.printOut();

  brain.fit(&inputsM, &outputsM, 5000);
  for (int i = 0; i < 4; i++) {
    Array input_Array(inputsM.data[i], 2);
    Array output_Array(outputsM.data[i], 1);
    input_Array.setName("input_Array");
    output_Array.setName("output_Array");
    input_Array.printOut();
    output_Array.printOut();
    Array testFeed(input_Array);
    brain.feedForward(&testFeed);
    Matrix testOutput(&brain.neurons[brain.numberOfLayers - 1], "testOutput");
    testOutput.printOut();
  }
  Serial.print("///////////////////////////////////////////////////////////////////////");

  if (false) {

  }
}

void loop() {
}
