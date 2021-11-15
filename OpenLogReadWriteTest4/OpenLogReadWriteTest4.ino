//#include <SoftwareSerial.h>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//Connect TXO of OpenLog to pin 3, RXI to pin 2
//SoftwareSerial(rxPin, txPin)
//SoftwareSerial OpenLog(3, 2); //Soft RX on 3, Soft TX out on 2
//UART OpenLog(3, 2);
int OpenLogIsInCommandMode = false;
int OpenLog_reset = 4; //This pin resets Serial1. Connect pin 4 to pin GRN on Serial1.

int statLED = 13;
