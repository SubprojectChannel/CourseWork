int myNN[] = {400, 20, 30, 8};
NeuralNetwork brain(myNN, sizeof(myNN) / sizeof(myNN[0]));

Queue IMUdata(myNN[0]);
int buttonPin = 4;

float actionThreshold = 0.3;
float savingThreshold = 0.9;
long detectionCooldown = 1000;

int HIDkeys[] = {int('n'), int('p'), 10, int(' '), 27, 127};

#define RED 22     
#define BLUE 24     
#define GREEN 23
#define LED_PWR 25
