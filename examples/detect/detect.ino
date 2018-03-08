/*
  Example code to control a generic RF433 power plug using a
  standard RF433 transmitter.

  https://coolcomponents.co.uk/products/rf-link-transmitter-434mhz
  https://www.amazon.co.uk/gp/product/B01ERN7GIE

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

#include <RF433Detector.h>

#define RF_DATA_PIN 2 // RF433 receiver plugged on pin 2

Detector detector(160, RF_DATA_PIN, 350, 2200);

void setup() {
  Serial.begin(38400);
  while (!Serial) {
    // wait serial port initialization
  }

  int sig1[46] = {1000,1500,500,500,500,500,500,500,1000,500,500,1000,1000,1000,1000,500,500,500,500,1000,500,500,500,500,500,500,500,500,500,500,500,500,500,500,500,500,1000,1000,500,500,500,500,1000,1000,500,500};
  int sig2[44] = {1000,1500,500,500,500,500,500,500,1000,500,500,1000,1000,1000,1000,500,500,500,500,1000,500,500,500,500,500,500,1000,1000,500,500,500,500,500,500,500,500,1000,1000,500,500,1000,1000,500,500};

  detector.addDetection(sig1, 46, 0.3);
  detector.addDetection(sig2, 44, 0.3);
  detector.start([]{detector.handleInterrupt();});
}

void loop() {
  bool detected[2] = {false, false};
  detector.checkDetected(detected);

  if (detected[0]) {
    Serial.println("Signal off detected");
  }

  if (detected[1]) {
    Serial.println("Signal on detected");
  }
}
