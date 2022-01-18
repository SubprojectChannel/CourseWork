int buttonPin = 4;

#define RED 22     
 #define BLUE 24     
 #define GREEN 23
 #define LED_PWR 25

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
}

int colour = 0;
boolean buttonWasPressed = false;

void loop() {
  boolean buttonIsPressed = !digitalRead(buttonPin);
  if(buttonIsPressed && !buttonWasPressed) colour = (colour+1)%8;
  buttonWasPressed = buttonIsPressed;
  
  digitalWrite(RED, 1-colour%2);
  digitalWrite(GREEN, 1-(colour/2)%2);
  digitalWrite(BLUE, 1-(colour/4)%2);

  delay(10);
}
