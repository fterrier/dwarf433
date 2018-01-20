# DwaRF433

Use this library to send arbitrary RF433 signals to control devices such as remote controlled plugs or certain heating systems (examples included).

This readme assumes you are familiar with RF433 and know the shape of the signal you want to transmit. If not, here are a few tutorials
I used to get started:

https://rayshobby.net/reverse-engineer-wireless-temperature-humidity-rain-sensors-part-1/
https://rayshobby.net/interface-with-remote-power-sockets-final-version/
https://rurandom.org/justintime/w/Cheapest_ever_433_Mhz_transceiver_for_PCs
https://www.laser.com/dhouston/learn.html

## Sending a signal

This library assumes a RF433 signal is a repetition of a pulse, which is formed of one or more waves. For example, the
power plugs mentioned in [power.ino](https://github.com/fterrier/dwarf433/blob/master/examples/power/power.ino) react to
the following signal:

```
- send 1000111001011101000000100
- wait 25 ms
- send 1000111001011101000000100
- wait 25 ms
- send 1000111001011101000000100
- wait 25 ms
- send 1000111001011101000000100
- wait 25 ms
... // repeat 20 times
- send 1000111001011101000000100
```

Which corresponds to the following:

```
Signal signal;
int repeatIntervals[19] = {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25};
signal.setRepeatIntervals(repeatIntervals, 19); // repeat intervals and number of intervals
signal.setPulse(Wave(0b1000111001011101000000100, 25)); // wave signal and length of wave
```

The 1s and 0s are transmitted using the specified encoding. For that example, the encoding is as follows:

Sending a 1 means setting the transmitter pin to `HIGH` for 200μs and to `LOW` for 450μs, while a 0 means setting
the transmitter pin to `HIGH` for 500μs and to `LOW` for 150μs.

To set the encoding, use the following:

```
Encoding encoding;
encoding.setZero(HIGH, 200, LOW, 450);
encoding.setOne(HIGH, 500, LOW, 150);
signal.setEncoding(encoding);
```

To send the signal, call the `sendSignal` function and specify the data PIN of your RF433 transmitter:

```
pinMode(RF_DATA_PIN, OUTPUT);
signal.sendSignal(RF_DATA_PIN);
```

## Hardware components

This library has been tested on the following hardware:
- Arduino/Genuino Uno
- ESP8266 (Huzzah)

The RF433 transmitter used was https://coolcomponents.co.uk/products/rf-link-transmitter-434mhz

## How to contribute

Feel free to create issues and pull requests in this repository.
