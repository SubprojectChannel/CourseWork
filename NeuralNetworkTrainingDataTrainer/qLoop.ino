
boolean buttonWasPressed = false;
boolean trainingMode = false;
void loop() {
  boolean buttonIsPressed = !digitalRead(buttonPin);
  if (!buttonWasPressed && buttonIsPressed) {
    trainingMode = !trainingMode;
  }
  buttonWasPressed = buttonIsPressed;

  if (trainingMode) {
    digitalWrite(LED_PWR, LOW);
    brain.loadRandomTrainingSamples();
    //brain.trainingInput.printOut();
    //brain.trainingOutput.printOut();
    //brain.fit(10, 1);
    //brain.saveNetworkToSD();

     //int sampleN = int(myRandom(0, trainingInput.rows, 1));
     int sampleN = 0;
     fitInputArray.setTo(brain.trainingInput.data[sampleN], brain.trainingInput.cols);
     fitOutputArray.setTo(brain.trainingOutput.data[sampleN], brain.trainingOutput.cols);
     fitInputArray.printOut();
     fitOutputArray.printOut();
     //brain.train(&fitInputArray, &fitOutputArray);
     //Serial.println(10 * brain.fullCost(&fitOutputArray));
  }
  else {
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
