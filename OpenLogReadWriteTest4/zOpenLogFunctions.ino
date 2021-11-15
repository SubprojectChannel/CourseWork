void OpenLog_RemoveFile(char *fileName) {
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  Serial1.print("rm ");
  Serial1.println(fileName);

  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '>') {}
}

//Open a file for writing. Begin writing at an offset
boolean OpenLog_OffsetWriteFile(char *fileName, int offset) {  //when printing text in append mode use '\r' for new line ( Serial1.print("Cats\rLike\rFish") )
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  Serial1.print("write ");
  Serial1.print(fileName);
  Serial1.print(" ");
  Serial1.print(offset);
  Serial1.write(13); //This is \r
  
  //The OpenLog echos the commands we send it by default so we have 'write log254.txt 10\r' (what we just sent) sitting
  //in the RX buffer. Let's try to ignore this.
  /* //original version of my code below
  while (1) {
    if (Serial1.available())
      if (Serial1.read() == '\r') break; //a fucking break
  }
*/
  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '\r') {}
  //OpenLog should respond with a < letting us know it's receiving characters

  /* //original version of my code below
  int counter = 0; ///change to for loop
  while (counter++ < 1000) {
    if (Serial1.available())
      if (Serial1.read() == '<') return (true);
    delay(1);
  }
  */
  
  //if we do not receive '<' in a second, it means that the file might not exist I guess (replacement to the code above)
  long t = millis();
  while(millis() - t < 1000) {
    if (Serial1.available())
      if (Serial1.read() == '<') return (true);
  }


  
  Serial.println("Write offset failed: Does the file exist?");
  return (false);
}





//This function creates a given file and then opens it in append mode (ready to record characters to the file)
//Then returns to listening mode
void OpenLog_CreateFile(char *fileName) {
  Serial1.print("new ");
  Serial1.println(fileName); //regular println works with OpenLog v2.51 and above

  //Wait for OpenLog to return to waiting for a command
  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '>') {}
}


void OpenLog_AppendFile(char* fileName) {  //when printing text in append mode use '\n' for new line ( Serial1.print("Cats\nLike\nFish") )
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
//is this fucking opening the file for writing?? why is it here??
  Serial1.print("append "); //hyposesis: "append", unlike "write", adds any data to the end of the file, instead of writing over it?.. but why \r then... 
  Serial1.print(fileName);
  Serial1.write(13); //This is \r
  OpenLogIsInCommandMode = false;
  //Wait for OpenLog to indicate file is open and ready for writing
  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '<') {}

  //OpenLog is now waiting for characters and will record them to the new file

}




//Reads the contents of a given file and dumps it to the serial terminal
//This function assumes the OpenLog is in command mode
void OpenLog_ReadFileIntoSerialPort(char *fileName) {
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  
  while(Serial1.available()) Serial1.read(); //Clear incoming buffer

  //Old way? idk, Ill cjeck
  Serial1.print("read ");
  Serial1.print(fileName);
  Serial1.write(13); //This is \r

  //New way?
  //Serial1.print("read ");
  //Serial1.println(fileName); //regular println works with OpenLog v2.51 and above

  //The OpenLog echos the commands we send it by default so we have 'read log823.txt\r' sitting
  //in the RX buffer. Let's try to not print this.
  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '\r') {}

  Serial.println("Reading from file:");

  //This will listen for characters coming from OpenLog and print them to the terminal
  //This relies heavily on the SoftSerial buffer not overrunning. This will probably not work
  //above 38400bps.
  //This loop will stop listening after 1 second of no characters received


//I actually did not look into this loop, so good luck with this attrocity
  for (int timeOut = 0 ; timeOut < 1000 ; timeOut++) {
    while (Serial1.available()) {
      char tempString[100];

      int spot = 0;
      while (Serial1.available()) {
        tempString[spot++] = Serial1.read();
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
  
  while(Serial1.available()) Serial1.read(); //Clear incoming buffer

  //Old way? idk, Ill cjeck
  Serial1.print("read ");
  Serial1.print(fileName);
  Serial1.write(13); //This is \r

  //New way?
  //Serial1.print("read ");
  //Serial1.println(fileName); //regular println works with OpenLog v2.51 and above

  //The OpenLog echos the commands we send it by default so we have 'read log823.txt\r' sitting
  //in the RX buffer. Let's try to not print this.
  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '\r') {}
}








//Check the stats of the SD card via 'disk' command
//This function assumes the OpenLog is in command mode
void OpenLog_ReadDisk() {//I don't know if this
  if(!OpenLogIsInCommandMode) OpenLog_gotoCommandMode();
  //Old way
  Serial1.print("disk");
  Serial1.write(13); //This is \r

  //I don't know if this line below is needed, haven't checked.
  ////OpenLogIsInCommandMode = false

  
  //New way
  //Serial1.print("read ");
  //Serial1.println(filename); //regular println works with OpenLog v2.51 and above

  //The OpenLog echos the commands we send it by default so we have 'disk\r' sitting
  //in the RX buffer. Let's try to not print this.
  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '\r') {}

  //This will listen for characters coming from OpenLog and print them to the terminal
  //This relies heavily on the SoftSerial buffer not overrunning. This will probably not work
  //above 38400bps.
  //This loop will stop listening after 1 second of no characters received
  for (int timeOut = 0 ; timeOut < 1000 ; timeOut++) {
    while (Serial1.available()) {
      char tempString[100];

      int spot = 0;
      while (Serial1.available()) {
        tempString[spot++] = Serial1.read();
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
  Serial1.begin(9600);

  //Reset OpenLog
  digitalWrite(OpenLog_reset, LOW);
  delay(10);
  digitalWrite(OpenLog_reset, HIGH);

  //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '<') {}
}


//This function pushes OpenLog into command mode
void OpenLog_gotoCommandMode(void) {
  //Send three control z to enter OpenLog command mode
  //Works with Arduino v1.0
  Serial1.write(26);
  Serial1.write(26);
  Serial1.write(26);

  //Wait for OpenLog to respond with '>' to indicate we are in command mode
  while (!Serial1.available()) {}
  while (Serial1.available() && Serial1.read() != '>') {}
  OpenLogIsInCommandMode = true;
}

void OpenLog_exitListeningMode() {
  Serial1.println("\r");
}
