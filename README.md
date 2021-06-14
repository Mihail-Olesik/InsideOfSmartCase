# SmartCase

This project is about smart case on base Raspberry Pi and Arduino Nano. This projects is consists of two parts: Arduino part and Raspberry part.

## Arduino part

Arduino is used to perform the controlling of engine. Default the data exchange rate with serial USB port is 115200. It's compatible with Arduino IDE.

## Raspberry part

Raspberry Pi is the main part of the project. It's the main computating center which sends commands to Arduino Nano, works with Bluetooth, GPS and different modules.
Computating possibilities of Raspberry Pi gives an opportunity to perform functions of Computer Vision. It's using in following the user. For following QR-code is used.
The encoding of using QR-code can be set at qr_text.txt file.

Scope of modules can be extended by adding the new module(-s) to file config.py placed at the root of RaspberryScript folder.
