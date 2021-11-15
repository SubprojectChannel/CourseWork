void freeMemory() {
  // allocate enough room for every thread's stack statistics
  int cnt = osThreadGetCount();
  mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));

  cnt = mbed_stats_stack_get_each(stats, cnt);
  for (int i = 0; i < cnt; i++) {
    printf("Thread: 0x%lX, Stack size: %lu / %lu\r\n", stats[i].thread_id, stats[i].max_size, stats[i].reserved_size);
  }
  free(stats);

  // Grab the heap statistics
  mbed_stats_heap_t heap_stats;
  mbed_stats_heap_get(&heap_stats);
  Serial.println("Heap size: " + String( heap_stats.current_size ) + " / " + String( heap_stats.reserved_size ));
}

float myMax(float a, float b) {
  if (a > b) return a;
  else return b;
}

float myRandom(float first, float last, int precision) {
  precision = max(precision, 1);
  float factor = pow(10, precision - 1);
  float r = random(0, (last - first) * factor);
  r /= factor;
  r += first;
  return r;
}

float normalize(float value, float maxValue) {
  value = constrain(value, -maxValue, maxValue) / (2 * maxValue) + 0.5;
  return value;
}

void saveTrainingDataToSD(String mySDFileName, float desiredOutput[], int desiredOutputLength) {
  if (SD.exists(mySDFileName)) SD.remove(mySDFileName); //delete file (I then create it so basically I just clear it)
  mySDFile = SD.open(mySDFileName, FILE_WRITE);
  mySDFile.print(String(IMUdata.queueSize)+","+String(desiredOutputLength)+",");
  for (int i = 0; i < IMUdata.queueSize; i++) {
    mySDFile.print(String(IMUdata.Data(i), 7) + ",");
  }
  for (int i = 0; i < desiredOutputLength; i++) {
    mySDFile.print(String(desiredOutput[i], 7) + ",");
  }
  mySDFile.close();
}
