#include <SoftwareSerial.h>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//Connect TXO of OpenLog to pin 3, RXI to pin 2
//SoftwareSerial(rxPin, txPin)
SoftwareSerial OpenLog(3, 2); //Soft RX on 3, Soft TX out on 2
int OpenLogIsInCommandMode = false;
int OpenLog_reset = 4; //This pin resets OpenLog. Connect pin 4 to pin GRN on OpenLog.

int statLED = 13;
