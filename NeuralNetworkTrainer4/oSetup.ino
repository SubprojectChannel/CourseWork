
void setup() {
  
  Serial.begin(9600);
  delay(3000);
  //while (!Serial);
  InitialiseSDCard();
  InitialiseIMU();
  memoryInfo();

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  LEDsOFF();
  
  //neural stuff
  randomSeed(analogRead(0));
  brain.learning_rate = 0.0002;
  Serial.println("Reading the network from sd card...");
  //brain.assignRandomNetwork();
  brain.readNetworkFromSD();
   //brain.saveNetworkToSD();
  //brain.printOut();
  Serial.println("Done reading the network from sd card...");
  
  IMUdata.Fill(0.5);

  memoryInfo();
  brain.prepareForTraining(60, 560);
  memoryInfo();
  
  Serial.println("setup done");

  
}
