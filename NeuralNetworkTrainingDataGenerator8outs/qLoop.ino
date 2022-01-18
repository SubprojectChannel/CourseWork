long timeOfLastSave = millis();
long timeBetweenSaves = 2000;
long lastTimeButtonWasNotPressed = millis();
int savedTrainings = 0;
boolean buttonWasPressed = false;
boolean wereDeletingAFile = false;

int lastRecordedOutputIndex = 0;
void loop() {
  listenForSerial();
  //LEDsStuff();
  
  boolean buttonIsPressed = !digitalRead(buttonPin);
  if(!buttonIsPressed) {
    lastTimeButtonWasNotPressed = millis();
    if(!buttonWasPressed) wereDeletingAFile = false;
  }
  if(buttonIsPressed && millis() - lastTimeButtonWasNotPressed > 1000 && lastRecodedTrainingIndex > -1) {
    removeLastRecordedTrainingData();
    lastTimeButtonWasNotPressed = millis();
    wereDeletingAFile = true;
  }
  else if(!buttonIsPressed && buttonWasPressed && !wereDeletingAFile) {
    int outputNeurons = brain.numberOfNeurons[brain.numberOfLayers-1];
    float desiredOutput[outputNeurons];
    for (int i = 0; i < outputNeurons; i++) {
      desiredOutput[i] = int(i == outputIndexBeingRecorded);
    }
    LEDsON(outputIndexBeingRecorded);
    saveTrainingDataToSD("td" + String(lastRecodedTrainingIndex+1) + ".txt", desiredOutput, sizeof(desiredOutput)/sizeof(desiredOutput[0]));
    lastRecodedTrainingIndex++;
    writeStringToFile("tdoff.txt", String(lastRecodedTrainingIndex) + + "\n" + String(outputIndexBeingRecorded));
    savedTrainings++;
    LEDsOFF();
    Serial.println(String(outputIndexBeingRecorded) + " - Saved trainings from power on: " + String(savedTrainings) + " at index " + String(lastRecodedTrainingIndex));
    timeOfLastSave = millis();
    //timeBetweenSaves = myRandom(1000, 2000, 3);
    lastRecordedOutputIndex = outputIndexBeingRecorded;
    //Serial.println("Saved trainings: " + String(savedTrainings) + " at " + String(savedTrainings-1+trainingIndexOffset));
  }
  /*
  else if(millis() - timeOfLastSave > timeBetweenSaves && false) {
    int outputNeurons = brain.numberOfNeurons[brain.numberOfLayers-1];
    float desiredOutput[outputNeurons];
    for (int i = 0; i < outputNeurons; i++) {
      desiredOutput[i] = int(i == 0);
    }
    saveTrainingDataToSD("td" + String(savedTrainings+trainingIndexOffset) + ".txt", desiredOutput, sizeof(desiredOutput)/sizeof(desiredOutput[0]));
    timeOfLastSave = millis();
    timeBetweenSaves = myRandom(1000, 2000, 3);
    savedTrainings++;
    Serial.println("Saved trainings: " + String(savedTrainings));
  }
  */
  float ax, ay, az;
  float gx, gy, gz;
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);
    ax = normalize(ax, 3);
    ay = normalize(ay, 3);
    az = normalize(az, 3);
    gx = normalize(gx, 700);
    gy = normalize(gy, 700);
    gz = normalize(gz, 700);
    IMUdata.deQueue(6);
    float newIMUdata[] = {ax, ay, az, gx, gy, gz};
    IMUdata.enQueue(newIMUdata, 6);
  }

  buttonWasPressed = buttonIsPressed;
}
