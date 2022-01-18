
int myNN[] = {400, 20, 30, 8};
NeuralNetwork brain(myNN, sizeof(myNN) / sizeof(myNN[0]));
//doesn't work when the top line in uncommented
//check how trainer2 is different from visualiser2 in the network class

Queue IMUdata(myNN[0]);
int buttonPin = 4;

#define RED 22     
 #define BLUE 24     
 #define GREEN 23
 #define LED_PWR 25
