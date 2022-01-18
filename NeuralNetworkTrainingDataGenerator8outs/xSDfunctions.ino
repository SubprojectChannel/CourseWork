String readShortFileToString(String mySDFileName) {
  String s = "";
  mySDFile = SD.open(mySDFileName);
  if (mySDFile) {
    while (mySDFile.available()) {
      char c = mySDFile.read();
      if (c != '\n') s += c;
    }
    mySDFile.close();
  }
  else Serial.println("error opening " + mySDFileName);
  return s;
}


void writeStringToFile(String mySDFileName, String text) {
  if (SD.exists(mySDFileName)) SD.remove(mySDFileName); //delete file (I then create it so basically I just clear it)
  mySDFile = SD.open(mySDFileName, FILE_WRITE);
  mySDFile.print(text);
  mySDFile.close();
}

void removeLastRecordedTrainingData() {
  String mySDFileName = "td" + String(lastRecodedTrainingIndex) + ".txt";
  if (SD.exists(mySDFileName)) SD.remove(mySDFileName);
  Serial.println("Removed file " + mySDFileName);
  lastRecodedTrainingIndex--;
  savedTrainings--;
  writeStringToFile("tdoff.txt", String(lastRecodedTrainingIndex)+"\n" + String(outputIndexBeingRecorded));
  for (int i = 1; i < 8; i++) {
    setLEDs(i % 2, (i / 2) % 2, (i / 4) % 2, 0);
    delay(50);
  }
  setLEDs(0, 0, 0, 0);
}
