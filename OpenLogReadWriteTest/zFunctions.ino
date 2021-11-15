

//Open a file for writing. Begin writing at an offset
boolean writeFile(char *fileName, int offset) {

  OpenLog.print("write ");
  OpenLog.print(fileName);
  OpenLog.print(" ");
  OpenLog.println(offset);
  //OpenLog.write(13); //This is \r //uncomment prob if doesnt  work



  //The OpenLog echos the commands we send it by default so we have 'write log254.txt 10\r' (what we just sent) sitting
  //in the RX buffer. Let's try to ignore this.
  //my version works?... Not sure
  //while (OpenLog.available() && OpenLog.read() != '\r') {}
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '\r') break; //a fucking break
  }

  //is this just checking whether '<' is in the first 1000 characters and if not assumes the file does not exist??
  //OpenLog should respond with a < letting us know it's receiving characters
  
  int counter = 0; ///change to for loop
  while (counter++ < 1000) {
    if (OpenLog.available())
      if (OpenLog.read() == '<') return (true);
    delay(1);
  }
  /*
  //if we do not receive '<' in a second, it means that the file might not exist I guess (replacement to the code above)
  long t = millis();
  while(millis() - t < 1000) {
    if (OpenLog.available())
      if (OpenLog.read() == '<') return (true);
  }
*/
  Serial.println("Write offset failed: Does the file exist?");
  return (false);
}





//This function creates a given file and then opens it in append mode (ready to record characters to the file)
//Then returns to listening mode
void createFile(char *fileName) {

  //Old way
  //OpenLog.print("new ");
  //OpenLog.print(fileName);
  //OpenLog.write(13); //This is \r

  //New way
  OpenLog.print("new ");
  OpenLog.println(fileName); //regular println works with OpenLog v2.51 and above

  //Wait for OpenLog to return to waiting for a command
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '>') break;
  }

  prepareForAppending(fileName);
}


void prepareForAppending(char* fileName) {
  
//is this fucking opening the file for writing?? why is it here??
  OpenLog.print("append "); //hyposesis: "append", unlike "write", adds any data to the end of the file, instead of writing over it?.. but why \r then... 
  OpenLog.print(fileName);
  OpenLog.write(13); //This is \r

  //Wait for OpenLog to indicate file is open and ready for writing
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '<') break;
  }

  //OpenLog is now waiting for characters and will record them to the new file

}




//Reads the contents of a given file and dumps it to the serial terminal
//This function assumes the OpenLog is in command mode
void readFile(char *fileName) {

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
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '\r') break;
  }

  Serial.println("Reading from file:");

  //This will listen for characters coming from OpenLog and print them to the terminal
  //This relies heavily on the SoftSerial buffer not overrunning. This will probably not work
  //above 38400bps.
  //This loop will stop listening after 1 second of no characters received
  Serial.println("start");
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
  Serial.println("end");

  //This is not perfect. The above loop will print the '.'s from the log file. These are the two escape characters
  //recorded before the third escape character is seen.
  //It will also print the '>' character. This is the OpenLog telling us it is done reading the file.

  //This function leaves OpenLog in command mode
}








//Check the stats of the SD card via 'disk' command
//This function assumes the OpenLog is in command mode
void readDisk() {

  //Old way
  OpenLog.print("disk");
  OpenLog.write(13); //This is \r

  //New way
  //OpenLog.print("read ");
  //OpenLog.println(filename); //regular println works with OpenLog v2.51 and above

  //The OpenLog echos the commands we send it by default so we have 'disk\r' sitting
  //in the RX buffer. Let's try to not print this.
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '\r') break;
  }

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
