void listenForSerial() {
  if(Serial.available()) {
    String numberString = "";
    char firstChar = ' ';
    while(Serial.available()) {
      char c = Serial.read();
      if(firstChar == ' ') firstChar = c;
      else if(c != '\n') numberString += c;
    }
    if(firstChar == 't') {
      lastRecodedTrainingIndex = numberString.toInt()-1;
      writeStringToFile("tdoff.txt", String(lastRecodedTrainingIndex) + "\n" + String(outputIndexBeingRecorded));
      Serial.println("next writing to td" + String(numberString.toInt()));
    }
    else if(firstChar == 'o') {
      outputIndexBeingRecorded = numberString.toInt();
      Serial.println("outputIndexBeingRecorded set to " + numberString);
    }
  }
}

void LEDsStuff() {
  if(millis() - timeOfLastSave < 300) {
    if(lastRecordedOutputIndex == 0) {
      setLEDs(0, 0, 0, 1);
    }
    else {
      int i = lastRecordedOutputIndex;
      setLEDs(i%2, (i/2)%2, (i/4)%2, 0);
    }
  }
  else setLEDs(0, 0, 0, 0);
}

void LEDsON(int i) {
    if(i == 0) {
      setLEDs(0, 0, 0, 1);
    }
    else {
      setLEDs(i%2, (i/2)%2, (i/4)%2, 0);
    }
}

void LEDsOFF() {
  setLEDs(0, 0, 0, 0);
}
