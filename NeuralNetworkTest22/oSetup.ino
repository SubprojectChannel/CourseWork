/*
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
*/

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  Serial.println(freeMemory());

  int myNN[] = {2, 2, 1};
  NeuralNetwork brain(myNN, sizeof(myNN)/sizeof(myNN[0]));

  float inputs[][2] = {
    {0, 0},
    {1, 0},
    {0, 1},
    {1, 1}
  };
  
  float outputs[][1] = {
    {0}, {1}, {1}, {0}
  };

  //convert inputs and outputs to matrix objects
  Matrix inputsM(sizeof(inputs)/sizeof(inputs[0]), sizeof(inputs[0])/sizeof(inputs[0][0]));
  Matrix outputsM(sizeof(outputs)/sizeof(outputs[0]), sizeof(outputs[0])/sizeof(outputs[0][0]));
  for(int i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++){
    for(int j = 0; j < sizeof(inputs[0])/sizeof(inputs[0][0]); j++) {
      inputsM.data[i][j] = inputs[i][j];
    }
    for(int j = 0; j < sizeof(outputs[0])/sizeof(outputs[0][0]); j++) {
      outputsM.data[i][j] = outputs[i][j];
    }
  }

  inputsM.setName("inputsM");
  outputsM.setName("outputsM");

  inputsM.printOut();
  outputsM.printOut();
  
  //Array testFeed(inputs[1], 2);
  //testFeed.setTo(brain.feedForward(&testFeed));
  //testFeed.printOut();
  //mem();
  //brain.fit(&inputsM, &outputsM, 1000);

  Array input_Array(inputs[1], 2);
  Array output_Array(outputs[1], 1);
  brain.train(&input_Array, &output_Array);
   

}

void loop() {
}
