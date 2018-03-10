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

#include "RF433Detector.h"
#include "list.h"
#include <Arduino.h>

Detector::Detector(int bs, int p) {
  stream = new List(bs);
  iterator = stream->iterator();
  pin = p;
  dtSize = 0;
  detectorTimings = new DetectorTiming[0];
  currentIndexes = new int[0];
}

Detector::~Detector() {
  delete stream;
  delete [] detectorTimings;
  delete [] currentIndexes;
}

void Detector::reset() {
  from = 0;
  to = 0;
  dtSize = 0;
  delete [] detectorTimings;
  delete [] currentIndexes;
  detectorTimings = new DetectorTiming[0];
  currentIndexes = new int[0];
}

void Detector::start(void (*callback)(void)) {
  attachInterrupt(digitalPinToInterrupt(pin), callback, CHANGE);
}

static unsigned int minimum(unsigned int *timings, unsigned int size) {
  unsigned int min = 0-1;
  for (int i=0;i<size;i++) {
    if (timings[i] < min) {
      min = timings[i];
    }
  }
  return min;
}

static unsigned int maximum(unsigned int *timings, unsigned int size) {
  unsigned int max = 0-1;
  for (int i=0;i<size;i++) {
    if (timings[i] > max) {
      max = timings[i];
    }
  }
  return max;
}

void Detector::addDetection(unsigned int *timings, unsigned int size, float tolerance) {
  DetectorTiming* dt = new DetectorTiming[dtSize+1];
  int* c = new int[dtSize+1];

  memcpy(dt, detectorTimings, dtSize*sizeof(DetectorTiming));
  memcpy(c, currentIndexes, dtSize*sizeof(int));

  delete [] detectorTimings;
  delete [] currentIndexes;

  detectorTimings = dt;
  currentIndexes = c;

  int* t = new int[size];
  memcpy(t, timings, size*sizeof(int));

  unsigned int mn = minimum(timings, size);
  unsigned int mx = maximum(timings, size);

  if (mn - mn*tolerance < from) {
    from = mn - mn*tolerance;
  }
  if (mx + mx*tolerance > to) {
    to = mx + mx*tolerance;
  }

  dt[dtSize] = DetectorTiming{t, size, tolerance};
  c[dtSize] = 0;
  dtSize = dtSize + 1;
}

void Detector::checkDetected(bool *detected) {
  if (iterator->atHead()) {
    for (int i=0;i<dtSize;i++) {
      currentIndexes[i] = 0;
    }
  }

  while (iterator->hasNext()) {
    int v = iterator->next();

    for (int i=0;i<dtSize;i++) {
      unsigned int c = currentIndexes[i];
      DetectorTiming dt = detectorTimings[i];
      int *sig = dt.timings;

      if (c < dt.size) {
        int mn = sig[c] - sig[c]*dt.tolerance;
        int mx = sig[c] + sig[c]*dt.tolerance;

        if (v < mx && v > mn) {
          currentIndexes[i] = c + 1;
        } else {
          currentIndexes[i] = 0;
        }
      }
      else {
        detected[i] = true;
        currentIndexes[i] = 0;
      }
    }
  }
}

void Detector::handleInterrupt() {
  volatile unsigned long now = micros();
  volatile unsigned int gap = now - last;

  // otherwise ignore
  if (gap > from && gap < to) {
    if (!stream->addInt(gap)) {
      Serial.println(".");
    }
  }
  last = now;
}
