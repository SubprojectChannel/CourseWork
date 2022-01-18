void InitialiseSDCard() {
  //Initializing the SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

void InitialiseIMU() {
  //Initializing IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void readLastTrainingIndexAndOutputIndex() {
  String mySDFileName = "tdoff.txt";
  boolean outputStarted = false;
  String trainingIndex = "";
  String outputIndex = "";
  mySDFile = SD.open(mySDFileName);
  if (mySDFile) {
    while (mySDFile.available()) {
      char c = mySDFile.read();
      if (c == '\n') outputStarted = true;
      else if(!outputStarted) {
        trainingIndex += c;
      }
      else outputIndex += c;
    }
    mySDFile.close();
    lastRecodedTrainingIndex = trainingIndex.toInt();
    outputIndexBeingRecorded = outputIndex.toInt();
    if(!digitalRead(buttonPin)) {
      outputIndexBeingRecorded = (outputIndexBeingRecorded+1)%8;
      writeStringToFile(mySDFileName, trainingIndex + "\n" + String(outputIndexBeingRecorded));
    }
  }
  else Serial.println("error opening " + mySDFileName);
}
