/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

File myFile;
String myFileName = "net.txt";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
/*
  if(SD.exists(myFileName)) SD.remove(myFileName); //delete file (I then create it so basically I just clear it
  myFile = SD.open(myFileName, FILE_WRITE);
  if (myFile) {
    myFile.println("you see it?\nThis is me\n\nCool, huh?");
    myFile.close();
  } else Serial.println("error opening " + myFileName);
*/
  String a = "";
  int i = 0;
  myFile = SD.open(myFileName);
  if (myFile) {
    Serial.println(myFileName);
    while (myFile.available()) {
      char c = myFile.read();
      Serial.print(c == '\n');
      i++;
      a+=c;
    }
    myFile.close();
  } else Serial.println("error opening " + myFileName);
  Serial.println(a);
}

void loop() {
  // nothing happens after setup
}
