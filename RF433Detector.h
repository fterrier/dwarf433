/*
  Rf433 - Arduino libary to transmit arbitrary RF433 signals
  Copyright (c) 2018 François Terrier.  All right reserved.

  Contributors:
  - François Terrier / fterrier(at)gmail(dot)com

  Project home: https://github.com/fterrier/dwarf433

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef RF433Detector_h
#define RF433Detector_h

#include "List.h"

struct DetectorTiming {
  unsigned int *timings;
  unsigned int size;
  float tolerance;
};

class Detector {
  public:
    Detector(int bufferSize, int pin, int from, int to);
    ~Detector();

    void addDetection(unsigned int *timings, unsigned int size, float tolerance);

    void start(void (*callback)(void));
    void handleInterrupt();

    void checkDetected(bool *detected);

  private:
    int pin, from, to;
    bool started;
    volatile unsigned long last;

    List* stream;
    Iterator* iterator;

    unsigned int dtSize;
    DetectorTiming* detectorTimings;
    unsigned int* currentIndexes;
};

#endif
