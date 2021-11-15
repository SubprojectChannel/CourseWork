int myNN[] = {400, 20, 20, 2};
NeuralNetwork brain(myNN, sizeof(myNN) / sizeof(myNN[0]));

Queue IMUdata(myNN[0]);

int buttonPin = 4;

#define RED 22     
 #define BLUE 24     
 #define GREEN 23
 #define LED_PWR 25
