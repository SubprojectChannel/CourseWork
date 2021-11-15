float OpenLog_findBeginning() {
  long lastTimeCharacterReceived = millis();
  boolean foundTheBeginning = false;
  while ((millis() - lastTimeCharacterReceived < 1000) && !foundTheBeginning) {
    while (Serial1.available() && !foundTheBeginning) {
      // Serial.println(Serial1.available());
      lastTimeCharacterReceived = millis();
      char c = Serial1.read();
      //String ssss =
      if (c == '#') foundTheBeginning = true;
    }
  }
}

float OpenLog_retrieveNextNumber() {
  long lastTimeCharacterReceived = millis();
  boolean numberDone = false;
  float theNumber = 0;
  int charIndex = -1;
  String numberString = "";
  while ((millis() - lastTimeCharacterReceived < 1000) && !numberDone) {
    while (Serial1.available() && !numberDone) {
      lastTimeCharacterReceived = millis();
      char c = Serial1.read();
      if (isADigit(c) || c == '-' || c == '.') numberString += c;
      else if (c == ',') {
        numberDone = true;
        theNumber = numberString.toFloat();
      }
    }
  }
  if(numberString == "") Serial.println("No numbers found");
  return theNumber;
}

boolean isADigit(char c) {
  return int(c) >= int('0') && int(c) <= int('9');
}


/*
  int rows = sizeof(data)/sizeof(data[0]);
  int cols = sizeof(data[0])/sizeof(data[0][0]);
  Serial1.print("S\n");
  Serial1.print("2,12,4\n");
  printAsAnArray(data, rows, cols);
  OpenLog_exitListeningMode(); //stop listening for data
*/
void printAsAnArray(float data[2][2], int rows, int cols) { //shit
  Serial1.print("weights0[" + String(rows) + "][" + String(cols) + "] = {\n");
  for (int i = 0; i < rows; i++) {
    Serial1.print("{");
    for (int j = 0; j < cols; j++) {
      Serial1.print(String(data[i][j], 7));
      if (j < cols - 1) Serial1.print(", ");
    }
    Serial1.print("}");
    if (i < rows - 1) Serial1.print(",");
    Serial1.print("\n");
  }
  Serial1.print("};\n");
}
