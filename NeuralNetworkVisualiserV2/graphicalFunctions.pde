
void drawNetwork(NeuralNetwork n) {
  fill(255);
  noStroke();
  //calculating neuron positions
  PVector neuronPosition[][] = new PVector[n.numberOfLayers][int(maxOfArray(float(n.numberOfNeurons)))];
  for (int layer = 0; layer < n.numberOfLayers; layer++) {
    for (int neuron = 0; neuron < n.numberOfNeurons[layer]; neuron++) {
      neuronPosition[layer][neuron] = new PVector(neuronDiameter*(layer+0.5)+neuronHorisontalSpacing*layer, neuronDiameter*(neuron+0.5)+neuronVerticalSpacing[layer]*(neuron+1));
    }
  }
  //drawing weights' lines
  strokeWeight(3*width/1000);
  for (int layer = n.numberOfLayers - 1; layer > 0; layer--) {
    for (int neuron = 0; neuron < n.numberOfNeurons[layer]; neuron++) {
      for (int previousNeuron = 0; previousNeuron < n.numberOfNeurons[layer-1]; previousNeuron++) {
        if(n.weights[layer-1].data[neuron][previousNeuron] < 0) stroke(255*(squishification(-n.weights[layer-1].data[neuron][previousNeuron])-0.5)*2, 0, 0);
        else stroke(0, 255*(squishification(n.weights[layer-1].data[neuron][previousNeuron])-0.5)*2, 0);
        line(neuronPosition[layer][neuron].x, neuronPosition[layer][neuron].y, neuronPosition[layer-1][previousNeuron].x, neuronPosition[layer-1][previousNeuron].y);
      }
    }
  }
  //drawing neurons
  stroke(255);
  for (int layer = 0; layer < n.numberOfLayers; layer++) {
    for (int neuron = 0; neuron < n.numberOfNeurons[layer]; neuron++) {
      fill(255*n.neurons[layer].data[neuron][0]);
      if(layer > 0) {
        if(n.biases[layer-1].data[neuron][0] < 0) stroke(255*(squishification(-n.biases[layer-1].data[neuron][0])-0.5)*2, 0, 0);
        else stroke(0, 255*(squishification(n.biases[layer-1].data[neuron][0])-0.5)*2, 0);
      } else stroke(255);
      circle(neuronPosition[layer][neuron].x, neuronPosition[layer][neuron].y, neuronDiameter);
      if(layer == n.numberOfLayers - 1) {
        fill(255*desiredOutput[neuron]);
        circle(neuronPosition[layer][neuron].x + 1.5*neuronDiameter, neuronPosition[layer][neuron].y, neuronDiameter);
      }
      
      /*
      //draw biases
      if(layer > 0) {
        textAlign(CENTER);
        textSize(32);
        if(biases[layer-1][neuron] < 0) fill(255*(squishification(-biases[layer-1][neuron])-0.5)*2, 0, 0);
        else fill(0, 255*(squishification(biases[layer-1][neuron])-0.5)*2, 0);
        text(biases[layer-1][neuron], neuronPosition[layer][neuron].x, neuronPosition[layer][neuron].y);
      }
      */
      /*
      //draw weights
      if(layer > 0) {
        textAlign(CENTER);
        textSize(32);
        if(weights[layer-1][neuron][0] < 0) fill(255*(squishification(-weights[layer-1][neuron][0])-0.5)*2, 0, 0);
        else fill(0, 255*(squishification(weights[layer-1][neuron][0])-0.5)*2, 0);
        text(weights[layer-1][neuron][0], neuronPosition[layer][neuron].x, neuronPosition[layer][neuron].y);
      }
      */
      
      /*
      //draw weight changes
      if(layer > 0) {
        textAlign(CENTER);
        textSize(32);
        if(dC0dw[layer-1][neuron][0][bunchSize] < 0) fill(255, 0, 0);
        else fill(0, 255, 0);
        text(dC0dw[layer-1][neuron][0][bunchSize]*100000, neuronPosition[layer][neuron].x, neuronPosition[layer][neuron].y);
      }
      */
    }
  }
  strokeWeight(1);
}

void drawWeightsValues(NeuralNetwork n) {
  //calculating neuron positions
  PVector neuronPosition[][] = new PVector[n.numberOfLayers][int(maxOfArray(float(n.numberOfNeurons)))];
  for (int layer = 0; layer < n.numberOfLayers; layer++) {
    for (int neuron = 0; neuron < n.numberOfNeurons[layer]; neuron++) {
      neuronPosition[layer][neuron] = new PVector(neuronDiameter*(layer+0.5)+neuronHorisontalSpacing*layer, neuronDiameter*(neuron+0.5)+neuronVerticalSpacing[layer]*(neuron+1));
    }
  }
  
  textAlign(CENTER);
  fill(150);
  textSize(32);
  PVector offset = new PVector(0, 0);
  for(int layer = 1; layer < n.numberOfLayers; layer++) {
    for(int neuron = 0; neuron < n.numberOfNeurons[layer]; neuron++) {
      for(int previousNeuron = 0; previousNeuron < n.numberOfNeurons[layer-1]; previousNeuron++) {
        PVector shift = new PVector ((neuronPosition[layer][neuron].x + neuronPosition[layer-1][previousNeuron].x)/2, (neuronPosition[layer][neuron].y + neuronPosition[layer-1][previousNeuron].y)/2);
        float angle = atan((neuronPosition[layer-1][previousNeuron].y - neuronPosition[layer][neuron].y)/(neuronPosition[layer-1][previousNeuron].x - neuronPosition[layer][neuron].x));
        translate(shift.x, shift.y);
        rotate(angle);
        if(angle > 0) offset.x += 120;
        if(angle < 0) offset.x -= 80;
        if(n.weights[layer-1].data[neuron][previousNeuron] < 0) fill(255*(squishification(-n.weights[layer-1].data[neuron][previousNeuron])-0.5)*2, 0, 0);
        else fill(0, 255*(squishification(n.weights[layer-1].data[neuron][previousNeuron])-0.5)*2, 0);
        text(n.weights[layer-1].data[neuron][previousNeuron], offset.x, offset.y);
        rotate(-angle);
        translate(-shift.x, -shift.y);
      }
    }
  }
}

void drawBiasesValues(NeuralNetwork n) {
  //calculating neuron positions
  PVector neuronPosition[][] = new PVector[n.numberOfLayers][int(maxOfArray(float(n.numberOfNeurons)))];
  for (int layer = 0; layer < n.numberOfLayers; layer++) {
    for (int neuron = 0; neuron < n.numberOfNeurons[layer]; neuron++) {
      neuronPosition[layer][neuron] = new PVector(neuronDiameter*(layer+0.5)+neuronHorisontalSpacing*layer, neuronDiameter*(neuron+0.5)+neuronVerticalSpacing[layer]*(neuron+1));
    }
  }
  
  for (int layer = 0; layer < n.numberOfLayers; layer++) {
    for (int neuron = 0; neuron < n.numberOfNeurons[layer]; neuron++) {
  if(layer > 0) {
        textAlign(CENTER);
        textSize(32);
        if(n.biases[layer-1].data[neuron][0] < 0) fill(255*(squishification(-n.biases[layer-1].data[neuron][0])-0.5)*2, 0, 0);
        else fill(0, 255*(squishification(n.biases[layer-1].data[neuron][0])-0.5)*2, 0);
        text(n.biases[layer-1].data[neuron][0], neuronPosition[layer][neuron].x, neuronPosition[layer][neuron].y);
      }
    }
  }
}
