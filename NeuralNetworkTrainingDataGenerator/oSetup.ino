void setup() {
  Serial.begin(9600);
  while (!Serial);
  InitialiseSDCard();
  InitialiseIMU();
  freeMemory();

  pinMode(buttonPin, INPUT_PULLUP);
  
  //neural stuff
  randomSeed(analogRead(0));
  brain.learning_rate = 0.2;
  //brain.readNetworkFromSD();

  IMUdata.Fill(0.5);

  //brain.saveNetworkToSD();
}
