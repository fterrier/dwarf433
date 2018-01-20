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

#include "rf433.h"
#include "Arduino.h"

Encoding::Encoding() {}

void Encoding::setZero(int o1, int t1, int o2, int t2) {
  zero[0] = t1;
  zero[1] = t2;
  zseq[0] = o1;
  zseq[1] = o2;
}

void Encoding::setOne(int o1, int t1, int o2, int t2) {
  one[0] = t1;
  one[1] = t2;
  oseq[0] = o1;
  oseq[1] = o2;
}

int* Encoding::getTimingsZero() {
  return zero;
}

int* Encoding::getOrderZero() {
  return zseq;
}

int* Encoding::getTimingsOne() {
  return one;
}

int* Encoding::getOrderOne() {
  return oseq;
}

Wave::Wave(String s, int len) {
  command = 0;

  for (int i=0;i<s.length();i++) {
    int index = s.length()-i-1;

    if (s[index] == '1') {
      command |= 1UL << i;
    }
  }

  len = s.length();
}

Wave::Wave(unsigned long c, int l) {
  command = c;
  len = l;
}

void Wave::sendWave(int pin, Encoding& encoding) {
  for (int i=1;i<=len;i++) {
    int b = (command >> (len-i)) & 1;

    int *delays = (b ? encoding.getTimingsOne() : encoding.getTimingsZero());
    int *seq = (b ? encoding.getOrderOne() : encoding.getOrderZero());

    digitalWrite(pin, seq[0]);
    delayMicroseconds(delays[0]);
    digitalWrite(pin, seq[1]);
    delayMicroseconds(delays[1]);
  }

  digitalWrite(pin, LOW);
}

Signal::Signal() {
  len = 0;
  encoding = Encoding();
}

Signal::~Signal() {
  delete [] delays;
  delete [] pulse.delays;
  delete [] pulse.seqs;
}

void Signal::setEncoding(Encoding e) {
  encoding = e;
}

void Signal::setPulse(Wave wave) {
  delete [] pulse.delays;
  delete [] pulse.seqs;

  pulse.len = 1;
  pulse.seqs = new Wave[1]{wave};
}

void Signal::setPulse(Wave wave1, int delay, Wave wave2) {
  delete [] pulse.delays;
  delete [] pulse.seqs;

  pulse.len = 2;
  pulse.delays = new int[1]{delay};
  pulse.seqs = new Wave[2]{wave1, wave2};
}

void Signal::setRepeatIntervals(int intervals[], int numElements) {
  delete [] delays;

  len = numElements + 1;
  delays = new int[numElements];
  memcpy(delays, intervals, numElements*sizeof(int));
}

void Signal::sendPulse(int pin, Encoding& encoding, Pulse& pulse) {
#if RF433_DEBUG == 1
  Serial.println("Sending pulse");
#endif

  for (int i=0;i<pulse.len;i++) {
    int d = (i == pulse.len-1) ? 0 : pulse.delays[i];

    pulse.seqs[i].sendWave(pin, encoding);
    delayMicroseconds(d);
  }
}

void Signal::sendSignal(int pin) {
#if RF433_DEBUG == 1
  Serial.println("Sending signal");
#endif

  for (int i=0;i<len;i++) {
    int sync = (i == len-1) ? 0 : delays[i];

    sendPulse(pin, encoding, pulse);

#if RF433_DEBUG == 1
    Serial.print("Waiting ms ");
    Serial.println(sync);
#endif

    keepDelay(sync);
  }
}

void Signal::keepDelay(int sync) {
  int decay = 20;

  int remaining = sync;
  while (remaining > 0) {
    if (decay < remaining) {
      delay(decay);
    } else {
      delay(remaining);
    }

    remaining = remaining - decay;
  }
}
