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

#include <rf433.h>

#define RF_DATA_PIN 12 // RF433 transmitter plugged on pin 12

Signal offSignal;
Signal onSignal;

bool activate = false;

void setup() {
  Serial.begin(38400);
  while (!Serial) {
    // wait serial port initialization
  }

  pinMode(RF_DATA_PIN, OUTPUT);

  Encoding encoding;
  encoding.setZero(HIGH, 200, LOW, 450);
  encoding.setOne(HIGH, 500, LOW, 150);

  int repeatIntervals[19] = {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25};
  offSignal.setRepeatIntervals(repeatIntervals, 19);
  offSignal.setEncoding(encoding);
  offSignal.setPulse(Wave(0b1000111001011101000000100, 25));

  onSignal.setRepeatIntervals(repeatIntervals, 19);
  onSignal.setEncoding(encoding);
  onSignal.setPulse(Wave(0b1000111001011101000010100, 25));
}

void loop() {
  if (activate == true) {
    onSignal.sendSignal(RF_DATA_PIN);
  }
  else {
    offSignal.sendSignal(RF_DATA_PIN);
  }

  activate = !activate;

  delay(5000);
}
