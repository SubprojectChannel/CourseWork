#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

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

void mem2() {
  Serial.print("FREE MEMORY: ");
  Serial.println(freeMemory());
}
