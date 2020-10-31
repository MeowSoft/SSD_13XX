#include <Arduino.h>
#include "benchmark/benchmark.h"

void setup() {
  
    // Init serial.
    Serial.begin(9600);
    while (!Serial) {;}

    Serial.println("Hello!!!");

    benchmark_setup();
    benchmark_run();
}

void loop() {
  // put your main code here, to run repeatedly:
}
