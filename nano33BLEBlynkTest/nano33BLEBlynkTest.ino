//WORKS
/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example shows how to use Arduino Nano 33 BLE
  to connect your project to Blynk.
  Note: This requires ArduinoBLE library
    from http://librarymanager/all#ArduinoBLE
    or https://github.com/arduino-libraries/ArduinoBLE
  NOTE: BLE support is in beta!
 *************************************************************/

#define BLYNK_PRINT Serial
#include <BlynkSimpleArduinoNanoBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "l_kuDJPUvfx9q_M94TleiArG2awbmrQ_";

int buttonPin = 3;
#define RED 22     
#define BLUE 24     
#define GREEN 23
#define LED_PWR 25

boolean buttonOn = false;

BLYNK_WRITE(V0) {buttonOn = boolean(param.asInt());}


void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  LEDsOFF();
  

  BLE.setAppearance(0x0080);
  Blynk.begin(auth, "Your mom");

  Serial.println("Waiting for connections...");

  
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  if (central) {    // If BLE is connected...
    Blynk.run();
    BLE.poll();
    LEDsON(int(buttonOn));
  }
}

void setLEDs(int r, int g, int b) {
  setLEDs(r, g, b, 0);
}

void setLEDs(int r, int g, int b, int p) {
  digitalWrite(RED, 1-r);
  digitalWrite(GREEN, 1-g);
  digitalWrite(BLUE, 1-b);
  digitalWrite(LED_PWR, 1-p);
}

void LEDsON(int i) {
    if(i == 0) {
      setLEDs(0, 0, 0, 1);
    }
    else {
      setLEDs(i%2, (i/2)%2, (i/4)%2, 0);
    }
}

void LEDsOFF() {
  setLEDs(0, 0, 0, 0);
}
