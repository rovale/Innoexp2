# Innoexp2 IoT

During this second Innnoexp we will explore creating physical 'Things' using [Arduinos](https://www.arduino.cc/en/Guide/Introduction) and similar boards and a wide range of sensors and actuators. Then we will connect them to the Internet of Things using the [MQTT](https://www.hivemq.com/mqtt-essentials/) protocol and [Node-RED](https://nodered.org/). The idea is to get up to speed with the technology as quickly as possible, to give room for creative thoughts and innovation to happen.

This repository contains some Arduino sketches and Node-Red flows used to introduce the basic concepts.

## Arduino sketches
Arduino boards are able to read inputs - light on a sensor, a finger on a button - and turn it into an output - activating a motor, turning on an LED. You can tell your board what to do by sending a set of instructions to the microcontroller on the board. To do so you use the [Arduino programming language](https://www.arduino.cc/reference/en/), and the [Arduino IDE](https://www.arduino.cc/en/main/software).

A sketch is the name that Arduino uses for a program. 

### Blink
This is the classic Arduino 'Hello World'.

It introduces:

- Usage of an [LED](https://learn.sparkfun.com/tutorials/light-emitting-diodes-leds).
- Usage of [resistors](https://learn.sparkfun.com/tutorials/resistors).
- The first [sketch](./Arduino/01-Blink/01-Blink.ino)
    - has a [setup and a loop](http://learn.olympiacircuits.com/setup-and-loop-blocks.html),
    - turns a [digital pin](https://www.arduino.cc/en/Tutorial/DigitalPins) [on and off](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/).
    - uses a [delay](https://www.arduino.cc/reference/en/language/functions/time/delay/) to time the blinking.

### Button
Turns the LED on when the button is pressed.

It introduces:
- The usage of a [button](https://learn.sparkfun.com/tutorials/switch-basics).
- [Pull-up resistors](https://learn.sparkfun.com/tutorials/pull-up-resistors).
- The [sketch](./Arduino/02-Button/02-Button.ino)
    - [reads](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/) the value of a digital pin.

### Photo resistor
Prints the measured value of a photo resistor to the serial output.

It introduces:
- The usage of a [photo resistor](https://learn.sparkfun.com/tutorials/photocell-hookup-guide), actually a light sensor.
- The [sketch](./Arduino/03-PhotoResistor/03-PhotoResistor.ino)
    - reads the value of an [analog input pin](https://www.arduino.cc/en/Tutorial/AnalogInputPins).
    - prints the value to the [serial](https://www.arduino.cc/reference/en/language/functions/communication/serial/) line.
    - uses a [non-blocking pattern](https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay) to time the print-outs.

### Json
Uses the serial interface. Sends the light sensor value every 500 seconds:
``` json
{"light": <0-1024>}
```

Sets the dim value of the LED after receiving:
``` json
{dimmer: <0-255>}
```

The [sketch](./Arduino/04-Json/04-Json.ino) introduces:
- usage of a [library](https://www.arduino.cc/en/Guide/Libraries) to create / parse json messages: [ArduinoJson](https://arduinojson.org).
- power modulation ([PWM](https://www.arduino.cc/en/Tutorial/PWM)) to dim the LED.

### BoatHorn
This [sketch](./Arduino/05-BoatHorn/05-BoatHorn.ino) used to connect an ESP-8266 directly to an MQTT broker and make it send status messages and respond on commands.

## Node-RED flows
Node-RED is a programming tool for wiring together hardware devices, APIs and online services in new and interesting ways.

It provides a browser-based editor that makes it easy to wire together flows using the wide range of nodes in the palette that can be deployed to its runtime in a single-click. Within NODE-Red it's also easy to create a live data [dashboard](https://github.com/node-red/node-red-dashboard).

All flows in this repository connect the Aruino through the serial port to Node-Red.

### Arduino Serial Json
Connects the Arduino to NODE-Red using the [Json](./Arduino/04-Json/04-Json.ino) sketch and [serialport](https://flows.nodered.org/node/node-red-node-serialport) nodes. On the NODE-Red dashboard it displays a gauge for the light intensity and a dimmer slider for the LED.

### Arduino Firmata
The same as the previous flow but now using the [Firmata](https://github.com/firmata/protocol) protocol with [Arduino](https://www.npmjs.com/package/node-red-node-arduino) nodes. It also adds a button to the dashboard to show the state of the button on the Arduino board.

### Arduino MQTT
The same as the previous flow but now directing all communication through an MQTT broker, turning our Arduino into a 'Thing' which is connected to the Internet of Things. This flow uses the builtin MQTT nodes. 

## Related information and links
- [Arduino](https://www.arduino.cc/en/Guide/Introduction)
- [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3)
- [Arduino Nano](https://store.arduino.cc/arduino-nano)
- [Arduino IDE](https://www.arduino.cc/en/main/software)
- [Driver](http://www.wch.cn/download/CH341SER_EXE.html) for [Nano V3.0 compatible](https://www.tinytronics.nl/shop/nl/arduino/main-boards/nano-v3.0-compatible?search=nano)
- [Arduino language reference](https://www.arduino.cc/reference/en/)
- [How to use a breadboard](https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard)
- [MQTT](https://www.hivemq.com/mqtt-essentials/)
- [Node-RED](https://nodered.org/)
