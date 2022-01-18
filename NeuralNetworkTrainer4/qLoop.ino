
boolean buttonWasPressed = false;
boolean trainingMode = true;
void loop() {
  boolean buttonIsPressed = !digitalRead(buttonPin);
  if (!buttonWasPressed && buttonIsPressed) {
    trainingMode = !trainingMode;
  }
  buttonWasPressed = buttonIsPressed;

  if (trainingMode) {
    setLEDs(1, 1, 1, 1);
    brain.loadRandomTrainingSamples();
    brain.fit(200, 5);
    brain.saveNetworkToSD();
  }
  else {
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

      Array testFeed(&IMUdata);
      brain.forwardPropagation(&testFeed);
      testFeed.remove();
      brain.neurons[brain.numberOfLayers - 1].printOut();
      int maxOutputIndex = 0;
      for(int i = 1; i < brain.neurons[brain.numberOfLayers - 1].rows; i++) {
        if(brain.neurons[brain.numberOfLayers - 1].data[i][0] > 
            brain.neurons[brain.numberOfLayers - 1].data[maxOutputIndex][0]) maxOutputIndex = i;
      }
      //output decision
      //LEDsStuff(maxOutputIndex);
      LEDsON(maxOutputIndex);
    }
  }


}
