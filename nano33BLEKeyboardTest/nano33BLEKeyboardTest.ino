///
///   ble_shining_kb.ino
#include "Nano33BleHID.h"
#include "signal_utils.h"
Nano33BleKeyboard bleKb("Shining Keyboard");

void setup() {
  Serial.begin(9600);
  
  // General setup.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize both BLE and the HID.
  bleKb.initialize();

  // Launch the event queue that will manage both BLE events and the loop.
  // After this call the main thread will be halted.
  MbedBleHID_RunEventThread();
}

int buttonPin = 4;
boolean buttonWasPressed = false;
int ci = 0;

void loop() {
  // When disconnected, we animate the builtin LED to indicate the device state.
  if (bleKb.connected() == false) {
    analogWrite(LED_BUILTIN, 255*abs(sin(float(TWO_PI)*0.0005*float(int(millis()%1000)))));
  }
  else {
    analogWrite(LED_BUILTIN, 30);
    // Retrieve the HIDService to update
    auto *kb = bleKb.hid();
    boolean buttonPressed = !digitalRead(buttonPin);
    if (buttonPressed && !buttonWasPressed) {
      int charCodes[] = {int('n'), int('p'), 10, 27, int(' '), int('w'), int('s'), int('a'), int('d'), 127};
      kb->sendCharacter(charCodes[ci]); // escape - 27 // enter - 10 // delete - 127
      ci = (ci+1) % (sizeof(charCodes)/sizeof(charCodes[0]));
    }
    buttonWasPressed = buttonPressed;
  }
}

/* -------------------------------------------------------------------------- */
