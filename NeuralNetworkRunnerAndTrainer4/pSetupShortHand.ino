
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

void InitialiseBLEHID() {
  // Initialize both BLE and the HID
  bleKb.initialize();

  // Launch the event queue that will manage both BLE events and the loop
  // After this call the main thread will be halted
  MbedBleHID_RunEventThread();
}

int readLastTrainingIndex() {
  String mySDFileName = "tdoff.txt";
  String trainingIndex = "";
  boolean doneReading = false;
  mySDFile = SD.open(mySDFileName);
  if (mySDFile) {
    while (mySDFile.available() && !doneReading) {
      char c = mySDFile.read();
      if (c == '\n') doneReading = true;
      if(!doneReading) trainingIndex += c;
    }
    mySDFile.close();
  }
  else {
    Serial.println("error opening " + mySDFileName);
    while(1);
  }
  return trainingIndex.toInt();
}
