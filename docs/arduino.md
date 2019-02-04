## Getting Started: Arduino

- Purchase an ESP8266 NodeMCU 12 or 12E | [eBay](http://www.ebay.com/itm/NodeMcu-Lua-WIFI-Internet-Things-development-board-based-ESP8266-CP2102-module-/201542946669?hash=item2eece54f6d:g:EOIAAOSw4q9XT5mo) (cheaper) or [Amazon.com](https://www.amazon.com/HiLetgo-Version-NodeMCU-Internet-Development/dp/B010O1G1ES/) (quicker)
- Purchase an OLED display | [eBay](http://www.ebay.com/itm/0-96-I2C-IIC-SPI-Serial-128X64-White-OLED-LCD-LED-Display-Module-for-Arduino-/201428440360?hash=item2ee6121528:g:MOYAAOSwv0tVeD0N), [Amazon.com](https://www.amazon
- Install Arduino
- Install libraries
	- Adafruit SSD1306
	- Adafruit GFX
	- Ace Button
- Add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to boards manager
- Select board profile: `NodeMCU 1.0 (ESP12-E Module)`
- install CP2102 driver https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

## Test Arduino Sketches

A few sketches are provided to test the hardware before uploading the SkyLift firmware

- once you have your PCB setup run the test Arduino sketches in `arduino/test`
- `test_buttons_serial` test the 3 buttons with serial
- `test_buttons_ace_serial` test the 3 buttons use the AceButton library
- `test_single_beacon_frame` test broadcasting a single beacon frame
- `test_multiple_beacon_frames` test broadcasting multiple beacon frames
- `test_oled_screen` test displaying text on the OLED screen