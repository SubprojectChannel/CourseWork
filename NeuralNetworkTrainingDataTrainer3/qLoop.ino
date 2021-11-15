
boolean buttonWasPressed = false;
boolean trainingMode = true;
void loop() {
  boolean buttonIsPressed = !digitalRead(buttonPin);
  if (!buttonWasPressed && buttonIsPressed) {
    trainingMode = !trainingMode;
  }
  buttonWasPressed = buttonIsPressed;

  if (trainingMode) {
    digitalWrite(LED_PWR, LOW);
    digitalWrite(BLUE, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    brain.loadRandomTrainingSamples();
    brain.fit(200, 5);
    brain.saveNetworkToSD();
  }
  else {
    digitalWrite(BLUE, LOW);
    digitalWrite(LED_PWR, HIGH);
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
      boolean rotation = brain.neurons[brain.numberOfLayers - 1].data[0][0] > 0.5;
      //output decision
      Serial.println(rotation);
      if (rotation) {
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, HIGH);
      }
      else {
        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, LOW);
      }
    }
  }


}
