void setup() {
  pinMode(statLED, OUTPUT);
  Serial.begin(9600);

  OpenLog_setup(); //Resets logger and waits for the '<' I'm alive character
  Serial.println("//////////////////////////////////////////////////////");
  Serial.println("OpenLog online");

  char* fileName = "network.txt"; //create a file name as an array of characters

  OpenLog_gotoCommandMode(); //Puts OpenLog in command mode
  OpenLog_CreateFile(fileName); //Creates the new file
  OpenLog_AppendFile(fileName); //puts into "append" mode, where every piece of data is added to the end of the file
  float data[2][2] = {
    {24, 56},
    {7.3, 81.2534}
  };

  //print data into the file
  
  int rows = sizeof(data)/sizeof(data[0]);
  int cols = sizeof(data[0])/sizeof(data[0][0]);
  OpenLog.print("#3,2,12,4,-1,");
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      if(float(int(data[i][j])) == data[i][j]) OpenLog.print(String(int(data[i][j])) + ",");
      else OpenLog.print(String(data[i][j], 7) + ",");
    }
  }

  OpenLog_exitListeningMode(); //stop listening for data


/*
//manual read
  OpenLog_ReadFile(fileName);
  Serial.println("Second reading from file:");
  long lastTimeCharacterReceived = millis();
  while (millis() - lastTimeCharacterReceived < 1000) {
    while (OpenLog.available()) {
      lastTimeCharacterReceived = millis();
      char character = OpenLog.read();
      Serial.print(character);
    }
  }
  Serial.println("Done reading again\n\n");

*/

  //Read data back
  float newData[2][2];
  OpenLog_ReadFile(fileName);
  Serial.println("Second reading from file:");
  OpenLog_findBeginning();
  Serial.println(OpenLog_retrieveNextNumber());
  Serial.println(OpenLog_retrieveNextNumber());

  Serial.println("Done reading again");
  /*
  while (millis() - lastTimeCharacterReceived < 1000) {
    while (OpenLog.available()) {
      lastTimeCharacterReceived = millis();
      char c = OpenLog.read();
      if(c == 'S') foundTheBeginning = true;
      if(foundTheBeginning) {
        if(c == 'w') {
      }
    }
  }
  Serial.println("Done reading again");
*/


/*
  
  //manual read
  OpenLog_ReadFile(fileName);
  Serial.println("Second reading from file:");
  long lastTimeCharacterReceived = millis();
  while (millis() - lastTimeCharacterReceived < 1000) {
    //Serial.println(OpenLog.available());
    while (OpenLog.available()) {
    //Serial.println(OpenLog.available());
      lastTimeCharacterReceived = millis();
      char character = OpenLog.read();
      Serial.print(character);
    }
  }
  Serial.println("Done reading again");
*/
  

  OpenLog_RemoveFile(fileName);
  Serial.println("Deleting done");
}
