long timeOfLastSave = millis();
long timeBetweenSaves = 2000;
int savedTrainings = 0;
boolean buttonWasPressed = false;

int trainingIndexOffset = 260;

void loop() {
  boolean buttonIsPressed = !digitalRead(buttonPin);
  if(!buttonWasPressed && buttonIsPressed) {
    float desiredOutput[] = {1, 0};
    saveTrainingDataToSD("td" + String(savedTrainings+trainingIndexOffset) + ".txt", desiredOutput, sizeof(desiredOutput)/sizeof(desiredOutput[0]));
    timeOfLastSave = millis();
    timeBetweenSaves = myRandom(200, 1500, 3);
    savedTrainings++;
    Serial.println("Saved trainings: " + String(savedTrainings));
  }
  else if(millis() - timeOfLastSave > timeBetweenSaves) {
    float desiredOutput[] = {0, 1};
    saveTrainingDataToSD("td" + String(savedTrainings+trainingIndexOffset) + ".txt", desiredOutput, sizeof(desiredOutput)/sizeof(desiredOutput[0]));
    timeOfLastSave = millis();
    timeBetweenSaves = myRandom(200, 1500, 3);
    savedTrainings++;
    Serial.println("Saved trainings: " + String(savedTrainings));
  }
  
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
