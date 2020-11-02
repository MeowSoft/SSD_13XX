#ifndef BENCHMARK_H
#define BENCHMARK_H

//#define USE_MINE

#ifdef USE_MINE
#include "SSD/SSD_13XX.h"
#else
#include <SSD_13XX.h>
#endif

#include "utils.h"

void benchmark_setup();
void benchmark_run();

#endif
