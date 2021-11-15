void setup() {
  pinMode(statLED, OUTPUT);
  Serial.begin(9600);

  OpenLog_setup(); //Resets logger and waits for the '<' I'm alive character
  Serial.println("//////////////////////////////////////////////////////");
  Serial.println("OpenLog online");

  char* fileName = "test.txt"; //create a file name as an array of characters

  OpenLog_gotoCommandMode(); //Puts OpenLog in command mode
  OpenLog_CreateFile(fileName); //Creates the new file

  Serial.print("File created: ");
  Serial.println(fileName);

  OpenLog_AppendFile(fileName); //puts into "append" mode, where every piece of data is added to the end of the file
  OpenLog.print("HELLO\nagain\n");
  OpenLog.print("and again");
  OpenLog_exitListeningMode(); //stop listening for data

//this puts into writing mode, where data starts being typed in
//at the offset index, other than the end of the file
//(if there aren't enough characters in the file, it returns (false)
  if (OpenLog_OffsetWriteFile(fileName, 5) == true) { //this puts into writing mode, where data starts at the offset, other than the end of the file
    OpenLog.print("Insert_");
    OpenLog.print("1st\r");
    OpenLog.print("Wow \rThis works?");
    OpenLog_exitListeningMode(); //exit listening mode and go to waiting for command mode
  }

  if (OpenLog_OffsetWriteFile(fileName, 12) == true) {
    OpenLog.print("HEY");
    OpenLog_exitListeningMode(); //exit listening mode and go to waiting for command mode
  }

  OpenLog_AppendFile(fileName); //puts into "append" mode, where every piece of data is added to the end of the file
  OpenLog.print("You see this?\n");
  OpenLog.print("Amazing");
  OpenLog_exitListeningMode(); //stop listening for data


  OpenLog_ReadFileIntoSerialPort(fileName); //This dumps the contents of a given file to the serial terminal

  Serial.println();
  Serial.println("File read complete");


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
  Serial.println("Done reading again");

  

  OpenLog_RemoveFile(fileName);
  Serial.println("Deleting done");
}
