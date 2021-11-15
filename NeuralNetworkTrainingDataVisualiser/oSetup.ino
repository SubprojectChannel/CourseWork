void setup() {
  Serial.begin(9600);
  while (!Serial);
  InitialiseSDCard();
  InitialiseIMU();
  freeMemory();

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  
  //neural stuff
  randomSeed(analogRead(0));
  brain.learning_rate = 0.2;
  //brain.readNetworkFromSD();

  IMUdata.Fill(0.5);

   //brain.saveNetworkToSD();

   brain.prepareForTraining(1, 270);
   int samples[] = {1};
   brain.loadTrainingSamples(samples);
}
