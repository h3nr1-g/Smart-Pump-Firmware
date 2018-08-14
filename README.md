# Smart Pump Firmware

## Summary

Smart Pump Firmware is the counter part of the project Smart Pump Backend (https://github.com/h3nr1-g/Smart-Pump-Backend).

This repository provides the source code for a ESP8266 micro controller allows the ESP to communicate with the backend system via the defined REST API.


## Brief Firmware Description

The ESP initialize it self and tries to establish a WiFi connection. If this is successful, the ESP downloads from the backend system two timing values. The first value is the amount of seconds which the ESP should pull the the preconfigured output pin to HIGH. After this time period the ESP goes into the deep sleep mode. The duration of the sleep mode if defined by second value. When the ESP wakes up, it starts with the initalization phase again.


## How To Set Up The Firmware

Execute the commands below in order to set up the project directory. The shell script setup.sh will do two things:
* Create a virtual Python environment with all the required tools for compiling and uploading of the firmware (platformio and esptool)
* Create a PlatformIO (https://platformio.org/) based project called "firmware"

```
sudo apt-get install virtualenv python-pip python-dev
git clone https://github.com/h3nr1-g/Smart-Pump-Firmware.git ~/sp-firmware
cd sp-firmware
./setup.sh
```  
You can activate the virtual Python environment via the command

```
source venv/bin/activate
```

This makes the commands "platformio" and "esptool" available for usage.

### Tip

You can modify line 41 of setup.sh in order to tell Platformio which IDE you want to use for code modification and extensions. The current predefined IDE is CLion (https://www.jetbrains.com/clion/).
