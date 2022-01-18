
boolean buttonWasPressed = false;

int fileIndex = 140;
boolean border = 0;
void loop() {
  boolean buttonIsPressed = !digitalRead(buttonPin);
  if(!buttonWasPressed && buttonIsPressed) {
    int samples[] = {fileIndex};
    fileIndex = (fileIndex+1)%brain.totalTrainingSamples;
    if(fileIndex == 0) fileIndex = 1;
    border = !border;
    brain.loadTrainingSamples(samples);
    for(int i = 0; i < brain.trainingInput.cols; i+=6) {
      Serial.print(int(border)*10);
      Serial.print(" ");
      for(int j = 0; j < 6; j++) {
        Serial.print(brain.trainingInput.data[0][i+j]*10);
        Serial.print(" ");
      }
    Serial.println("");
    }
  }
  buttonWasPressed = buttonIsPressed;

  int outputIndex = 0;
  for(int i = 0; i < brain.trainingOutput.cols; i++) {
    if(brain.trainingOutput.data[0][i] == 1) outputIndex = i;
  }
  //Serial.println(outputIndex);
  LEDsStuff(outputIndex);
  
  delay(10);
  
}
