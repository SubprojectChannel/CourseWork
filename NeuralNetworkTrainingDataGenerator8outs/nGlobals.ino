
float inputs[][5] = {
  {0.21386111, 0.94638675, 0.1548028, 0.20960897, 0.044149816},
  {0.7477825, 0.5254611, 0.7469225, 0.9514254, 0.59847575},
  {0.021902263, 0.32995826, 0.45149916, 0.7496793, 0.3465472},
  {0.6198675, 0.6255245, 0.5761538, 0.8088562, 0.941487},
  {0.21515805, 0.31365532, 0.8744001, 0.40784645, 0.9485363},
  {0.6182955, 0.42888337, 0.1595555, 0.55932415, 0.75027645},
  {0.17368764, 0.7824764, 0.1501233, 0.8570599, 0.3336358}
};

float outputs[][2] = {
  {0.0, 1.0},
  {1.0, 0.0},
  {0.0, 1.0},
  {1.0, 0.0},
  {1.0, 0.0},
  {0.0, 1.0},
  {0.0, 1.0}
};

/*
  float inputs[4][2] = {
    {0, 0},
    {1, 0},
    {0, 1},
    {1, 1}
  };

  float outputs[4][2] = {
    {0, 0}, {1, 1}, {1, 1}, {0, 0}
  };
*/

//int myNN[] = {sizeof(inputs[0]) / sizeof(inputs[0][0]), 6, sizeof(outputs[0]) / sizeof(outputs[0][0])};
//NeuralNetwork brain(myNN, sizeof(myNN) / sizeof(myNN[0]));

int myNN[] = {400, 8};
NeuralNetwork brain(myNN, sizeof(myNN) / sizeof(myNN[0]));

Matrix inputsM(sizeof(inputs) / sizeof(inputs[0]), sizeof(inputs[0]) / sizeof(inputs[0][0]), "inputsM");
Matrix outputsM(sizeof(outputs) / sizeof(outputs[0]), sizeof(outputs[0]) / sizeof(outputs[0][0]), "outputsM");

Queue IMUdata(myNN[0]);

int buttonPin = 4;
#define RED 22     
#define BLUE 24     
#define GREEN 23
#define LED_PWR 25
 
int outputIndexBeingRecorded = 1;
