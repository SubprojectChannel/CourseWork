void OpenLog_RemoveFile(char *fileName) {
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  OpenLog.print("rm ");
  OpenLog.println(fileName);

  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '>') {}
}

//Open a file for writing. Begin writing at an offset
boolean OpenLog_OffsetWriteFile(char *fileName, int offset) {  //when printing text in append mode use '\r' for new line ( OpenLog.print("Cats\rLike\rFish") )
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  OpenLog.print("write ");
  OpenLog.print(fileName);
  OpenLog.print(" ");
  OpenLog.print(offset);
  OpenLog.write(13); //This is \r
  
  //The OpenLog echos the commands we send it by default so we have 'write log254.txt 10\r' (what we just sent) sitting
  //in the RX buffer. Let's try to ignore this.
  /* //original version of my code below
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '\r') break; //a fucking break
  }
*/
  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '\r') {}
  //OpenLog should respond with a < letting us know it's receiving characters

  /* //original version of my code below
  int counter = 0; ///change to for loop
  while (counter++ < 1000) {
    if (OpenLog.available())
      if (OpenLog.read() == '<') return (true);
    delay(1);
  }
  */
  
  //if we do not receive '<' in a second, it means that the file might not exist I guess (replacement to the code above)
  long t = millis();
  while(millis() - t < 1000) {
    if (OpenLog.available())
      if (OpenLog.read() == '<') return (true);
  }


  
  Serial.println("Write offset failed: Does the file exist?");
  return (false);
}





//This function creates a given file and then opens it in append mode (ready to record characters to the file)
//Then returns to listening mode
void OpenLog_CreateFile(char *fileName) {
  OpenLog.print("new ");
  OpenLog.println(fileName); //regular println works with OpenLog v2.51 and above

  //Wait for OpenLog to return to waiting for a command
  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '>') {}
}


void OpenLog_AppendFile(char* fileName) {  //when printing text in append mode use '\n' for new line ( OpenLog.print("Cats\nLike\nFish") )
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
//is this fucking opening the file for writing?? why is it here??
  OpenLog.print("append "); //hyposesis: "append", unlike "write", adds any data to the end of the file, instead of writing over it?.. but why \r then... 
  OpenLog.print(fileName);
  OpenLog.write(13); //This is \r
  OpenLogIsInCommandMode = false;
  //Wait for OpenLog to indicate file is open and ready for writing
  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '<') {}

  //OpenLog is now waiting for characters and will record them to the new file

}




//Reads the contents of a given file and dumps it to the serial terminal
//This function assumes the OpenLog is in command mode
void OpenLog_ReadFileIntoSerialPort(char *fileName) {
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  
  while(OpenLog.available()) OpenLog.read(); //Clear incoming buffer

  //Old way? idk, Ill cjeck
  OpenLog.print("read ");
  OpenLog.print(fileName);
  OpenLog.write(13); //This is \r

  //New way?
  //OpenLog.print("read ");
  //OpenLog.println(fileName); //regular println works with OpenLog v2.51 and above

  //The OpenLog echos the commands we send it by default so we have 'read log823.txt\r' sitting
  //in the RX buffer. Let's try to not print this.
  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '\r') {}

  Serial.println("Reading from file:");

  //This will listen for characters coming from OpenLog and print them to the terminal
  //This relies heavily on the SoftSerial buffer not overrunning. This will probably not work
  //above 38400bps.
  //This loop will stop listening after 1 second of no characters received


//I actually did not look into this loop, so good luck with this attrocity
  for (int timeOut = 0 ; timeOut < 1000 ; timeOut++) {
    while (OpenLog.available()) {
      char tempString[100];

      int spot = 0;
      while (OpenLog.available()) {
        tempString[spot++] = OpenLog.read();
        if (spot > 98) break;
      }
      tempString[spot] = '\0';
      Serial.write(tempString); //Take the string from OpenLog and push it to the Arduino terminal
      timeOut = 0;
    }
    delay(1);
  }
  //This is not perfect. The above loop will print the '.'s from the log file. These are the two escape characters
  //recorded before the third escape character is seen.
  //It will also print the '>' character. This is the OpenLog telling us it is done reading the file.

  //This function leaves OpenLog in command mode
}


void OpenLog_ReadFile(char *fileName) {
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  
  while(OpenLog.available()) OpenLog.read(); //Clear incoming buffer

  //Old way? idk, Ill cjeck
  OpenLog.print("read ");
  OpenLog.print(fileName);
  OpenLog.write(13); //This is \r

  //New way?
  //OpenLog.print("read ");
  //OpenLog.println(fileName); //regular println works with OpenLog v2.51 and above

  //The OpenLog echos the commands we send it by default so we have 'read log823.txt\r' sitting
  //in the RX buffer. Let's try to not print this.
  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '\r') {}
}








//Check the stats of the SD card via 'disk' command
//This function assumes the OpenLog is in command mode
void OpenLog_ReadDisk() {//I don't know if this
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  //Old way
  OpenLog.print("disk");
  OpenLog.write(13); //This is \r

  //I don't know if this line below is needed, haven't checked.
  ////OpenLogIsInCommandMode = false

  
  //New way
  //OpenLog.print("read ");
  //OpenLog.println(filename); //regular println works with OpenLog v2.51 and above

  //The OpenLog echos the commands we send it by default so we have 'disk\r' sitting
  //in the RX buffer. Let's try to not print this.
  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '\r') {}

  //This will listen for characters coming from OpenLog and print them to the terminal
  //This relies heavily on the SoftSerial buffer not overrunning. This will probably not work
  //above 38400bps.
  //This loop will stop listening after 1 second of no characters received
  for (int timeOut = 0 ; timeOut < 1000 ; timeOut++) {
    while (OpenLog.available()) {
      char tempString[100];

      int spot = 0;
      while (OpenLog.available()) {
        tempString[spot++] = OpenLog.read();
        if (spot > 98) break;
      }
      tempString[spot] = '\0';
      Serial.write(tempString); //Take the string from OpenLog and push it to the Arduino terminal
      timeOut = 0;
    }

    delay(1);
  }

  //This is not perfect. The above loop will print the '.'s from the log file. These are the two escape characters
  //recorded before the third escape character is seen.
  //It will also print the '>' character. This is the OpenLog telling us it is done reading the file.

  //This function leaves OpenLog in command mode
}

void OpenLog_setup(void) {
  pinMode(OpenLog_reset, OUTPUT);
  OpenLog.begin(9600);

  //Reset OpenLog
  digitalWrite(OpenLog_reset, LOW);
  delay(10);
  digitalWrite(OpenLog_reset, HIGH);

  //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '<') {}
}


//This function pushes OpenLog into command mode
void OpenLog_gotoCommandMode(void) {
  //Send three control z to enter OpenLog command mode
  //Works with Arduino v1.0
  OpenLog.write(26);
  OpenLog.write(26);
  OpenLog.write(26);

  //Wait for OpenLog to respond with '>' to indicate we are in command mode
  while (!OpenLog.available()) {}
  while (OpenLog.available() && OpenLog.read() != '>') {}
  OpenLogIsInCommandMode = true;
}

void OpenLog_exitListeningMode() {
  OpenLog.println("\r");
}
