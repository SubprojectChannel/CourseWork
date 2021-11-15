void setup() {
  pinMode(statLED, OUTPUT);
  Serial.begin(9600);

  setupOpenLog(); //Resets logger and waits for the '<' I'm alive character
  Serial.println("OpenLog online");

  char* fileName = "lmao23.txt";
  //sprintf(fileName, "log%03d.txt", fileNumber);

  gotoCommandMode(); //Puts OpenLog in command mode
  //OpenLog.println('?');
  createFile(fileName);
  
  Serial.print("Random file created: ");
  Serial.println(fileName);

  gotoCommandMode(); //Puts OpenLog in command mode



  
  boolean poop = writeFile(fileName, 0);
  OpenLog.print("Starting statement");
  for (int x = 0 ; x < 4 ; x++) OpenLog.print(char(int('u')+x));
 // OpenLog.write(13);

  //gotoCommandMode();
  boolean poop2 = writeFile(fileName, 9);
  OpenLog.print("secondary statement");
  for (int x = 0 ; x < 4 ; x++) OpenLog.print(char(int('u')+x));
  OpenLog.print("\r");

  //Now write to the same file but at a specific location
  //gotoCommandMode(); //Puts OpenLog in command mode
  if (writeFile(fileName, 2) == true) //Write to fileName starting at location 10
  {
    //Write something to this file at location 10
    OpenLog.print("yes");

    //The write command will continue to draw in characters until a blank line is received
   OpenLog.print("\r");

    //OpenLog is now returned to the command prompt
  }
OpenLog.print("\r");
  //Now let's read back the file to see what it looks like

  //gotoCommandMode();
  readFile(fileName); //This dumps the contents of a given file to the serial terminal

  Serial.println();
  Serial.println("File read complete");
}
