float myMax(float a, float b) {
  if(a > b) return a;
  else return b;
}

float myRandom(float first, float last, int precision) {
  precision = max(precision, 1);
  float factor = pow(10, precision-1);
  float r = random(0, (last-first)*factor);
  r /= factor;
  r += first;
  return r;
}

void mem() {
  Serial.print("free memory: ");
  Serial.println(freeMemory());
}

int freeMemory() {
  return 1;
}
