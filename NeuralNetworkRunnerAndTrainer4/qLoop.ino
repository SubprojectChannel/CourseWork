long lastTimeAMovementDetected = 0;
boolean buttonWasPressed = false;
boolean trainingMode = false;

void loop() {
  if (!bleKb.connected()) {
    //blinking cyan
    analogWrite(BLUE, 255 * abs(sin(float(TWO_PI) * 0.0005 * float(int(millis() % 1000)))));
    analogWrite(GREEN, 255 * abs(sin(float(TWO_PI) * 0.0005 * float(int(millis() % 1000)))));
  }
  else {
    // Retrieve the HIDService to update
    auto *kb = bleKb.hid();

    //checking in with the button
    boolean buttonIsPressed = !digitalRead(buttonPin);
    if (!buttonWasPressed && buttonIsPressed) {
      trainingMode = !trainingMode;
    }
    buttonWasPressed = buttonIsPressed;

    if (trainingMode) {
      setLEDs(1, 1, 1, 1);
      brain.loadRandomTrainingSamples();
      brain.fit(5, 1);
      brain.saveNetworkToSD();
    }
    else {
      float ax, ay, az;
      float gx, gy, gz;
      if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
        //objective 1.a
        IMU.readAcceleration(ax, ay, az);
        IMU.readGyroscope(gx, gy, gz);
        float accMax = 3, gyroMax = 700;
        ax = normalize(ax, accMax);
        ay = normalize(ay, accMax);
        az = normalize(az, accMax);
        gx = normalize(gx, gyroMax);
        gy = normalize(gy, gyroMax);
        gz = normalize(gz, gyroMax);
        IMUdata.deQueue(6);
        float newIMUdata[] = {ax, ay, az, gx, gy, gz};
        IMUdata.enQueue(newIMUdata, 6);

        Array feed(&IMUdata);
        brain.forwardPropagation(&feed); //objective 1.b
        feed.remove();
        //brain.neurons[brain.numberOfLayers - 1].printOut();
        int maxOutputIndex = 0;
        for (int i = 1; i < brain.neurons[brain.numberOfLayers - 1].rows; i++) {
          if (brain.neurons[brain.numberOfLayers - 1].data[i][0] >
              brain.neurons[brain.numberOfLayers - 1].data[maxOutputIndex][0]) maxOutputIndex = i;
        }
        float maxOutputValue = brain.neurons[brain.numberOfLayers - 1].data[maxOutputIndex][0];

        //objective 1.c
        //if the network thinks it might be a movement detected
        if (maxOutputIndex != 0 && millis() - lastTimeAMovementDetected > detectionCooldown) {
          //if the the network is certain enough to act, trigger the required action
          if (maxOutputValue > actionThreshold) {
            lastTimeAMovementDetected = millis();
            kb->sendCharacter(HIDkeys[maxOutputIndex - 1]); //objective 7.b
            LEDsON(maxOutputIndex);
          }
          //if the the network is certain enough to store the data as input in its output, trigger the required action
          if (maxOutputValue > savingThreshold) {
            float desiredOutput[brain.trainingOutput.cols];
            desiredOutput[maxOutputIndex] = 1;
            saveTrainingDataToSD("td" + String(brain.totalTrainingSamples) + ".txt", desiredOutput, sizeof(desiredOutput)/sizeof(desiredOutput[0]));
            writeStringToFile("tdoff.txt", String(brain.totalTrainingSamples) + "\n" + String(maxOutputIndex));
            brain.totalTrainingSamples++;
          }
        }
        else LEDsON(0);
      }
    }
  }
}
