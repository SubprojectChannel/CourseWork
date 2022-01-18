int lastRecodedTrainingIndex = 0;

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  InitialiseSDCard();
  InitialiseIMU();
  freeMemory();

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  LEDsOFF();
  
  //neural stuff
  randomSeed(analogRead(0));
  brain.learning_rate = 0.2;
  //brain.readNetworkFromSD();

  IMUdata.Fill(0.5);

  //brain.saveNetworkToSD();

  readLastTrainingIndexAndOutputIndex();
  for(int i = 0; i < 5; i++) {
    LEDsON(outputIndexBeingRecorded);
    delay(50);
    LEDsOFF();
    delay(50);
  }
  //lastRecodedTrainingIndex = readShortFileToString("tdoff.txt").toInt();
  //Serial.println("trainingIndexOffset read: " + readShortFileToString("tdoff.txt"));
}
