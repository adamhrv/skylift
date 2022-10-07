# SkyLift

## Setup

Python	
- `python -m venv venv`
- `source venv/bin/activate`
- `pip install -r requirements.txt`
- `cd skylift/cli/`
- Test: `python cli.py --help` 

Setup ESP	
- Install boards in File > Preferences: Additional boards manager URLs
- board URLs `http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
- Select board:
	- NodeMCU ESP32 is "ESP32 Dev Module"
- Set upload speed in Tools > Upload Speed to 460800

## Usage

Convert WiFi/BT scan to networks JSON
```python
python cli.py convert-scan 
	-i path/to/scan.csv
	-o path/to/scan.json
```

Create Arduino sketch from template
```python
python cli.py create-sketch
	-i path/to/scan.json
	-o path/to/scan-arduino/
```

Customize more parameters
```
```python
python cli.py create-sketch
	-i path/to/scan.json
	-o path/to/scan-arduino/
	--max-networks 12
	--type esp32
```




## Troubleshooting

Linux
- Ensure USB serial is allowed `sudo chmod a+rw /dev/ttyUSB0`
- Ensure pyserial is installed `pip install pyserial --user`


## TODO
- [ ] add BLE adverts
- [ ] review and confirm WiFi packet structure
- [ ] add randomness to WiFi packets
- [ ] add Wigle API
- [ ] add emoji SSID support 🙃