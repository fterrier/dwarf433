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

#ifndef RF433_h
#define RF433_h

#define RF433_DEBUG 0

#include <Arduino.h>

class Encoding {
  public:
    Encoding();

    void setZero(int o1, int t1, int o2, int t2);
    void setOne(int o1, int t1, int o2, int t2);

    int* getTimingsZero();
    int* getOrderZero();
    int* getTimingsOne();
    int* getOrderOne();

  private:
    int zero[2];
    int zseq[2];
    int one[2];
    int oseq[2];
};

class Wave {
  public:
    Wave();
    Wave(String command);
    Wave(unsigned long command, int len);

    void sendWave(int pin, Encoding& encoding);
    String toString();

  private:
    int len;
    unsigned long command;
};

class Signal {
  public:
    Signal();
    ~Signal();
    void setPulse(Wave wave);
    void setPulse(Wave wave1, int delay, Wave wave2);
    void setPulse(Wave waves[], int delays[], int numWaves);
    void setEncoding(Encoding encoding);
    void setRepeatIntervals(int intervals[], int numIntervals);

    void sendSignal(int pin);

  private:

    struct Pulse {
      int len;
      Wave *seqs;
      int *delays;
    };

    void sendPulse(int pin, Pulse& pulse);
    void keepDelay(int sync);

    int len;
    Pulse pulse;
    Encoding encoding;
    int *delays;
};

#endif
