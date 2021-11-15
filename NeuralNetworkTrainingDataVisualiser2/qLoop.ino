
boolean buttonWasPressed = false;

int fileIndex = 2;
boolean border = 0;



Queue yourMom(228);
int c = 0;
void loop() {
  if(c == 0) {
    yourMom.remove();
    c++;
  }
  boolean buttonIsPressed = !digitalRead(buttonPin);
  if(!buttonWasPressed && buttonIsPressed) {
    int samples[] = {fileIndex};
    fileIndex = (fileIndex+1)%brain.totalTrainingSamples;
    if(fileIndex == 0) fileIndex = 1;
    border = !border;
    brain.loadTrainingSamples(samples);
    for(int i = 0; i < brain.trainingInput.cols; i+=6) {
      Serial.print(int(border));
      Serial.print(" ");
      for(int j = 0; j < 6; j++) {
        Serial.print(brain.trainingInput.data[0][i+j]);
        Serial.print(" ");
      }
    Serial.println("");
    }
  }
  buttonWasPressed = buttonIsPressed;

  if(brain.trainingOutput.data[0][0] == 1) {
    digitalWrite(RED, LOW); 
    digitalWrite(GREEN, HIGH);
  }
  else {
    digitalWrite(RED, HIGH); 
    digitalWrite(GREEN, LOW);
  }
  delay(10);
  
}
