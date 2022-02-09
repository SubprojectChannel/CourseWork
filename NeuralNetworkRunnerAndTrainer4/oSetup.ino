
void setup() {
  //initialize serial communication
  Serial.begin(9600);
  
  InitialiseSDCard();
  InitialiseIMU();

  //initialise LED and button pin modes
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  LEDsOFF();
  
  randomSeed(analogRead(0));
  brain.learning_rate = 0.0002;
  Serial.println("Reading the network from sd card...");
  brain.readNetworkFromSD();
  Serial.println("Done reading the network from sd card...");

  IMUdata.Fill(0.5);
  
  readLastTrainingIndex();//read the last saved training data index
  brain.prepareForTraining(60, readLastTrainingIndex()+1);//load the maximum amount of training data that the Arduino's RAM would allow for
  
  Serial.println("setup done");
  InitialiseBLEHID(); //objective 7.a
}
