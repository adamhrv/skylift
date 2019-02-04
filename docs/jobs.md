# Jobs

SkyLift uses CSV *job* files to batch process data

## Wigle API Jobs

- run `python cli_jobs.py wigle_api -j ../data/jobs/wigle_api.csv`
- this will generate JSON files in the `data/networks` directory unless otherwise specified

| run | filepath | lat | lon | radius | since | comment |
|---|---|---|---|---|---|---
| 1 | data/networks/fb\_central.json | 37.4845432 | -122.1493071 | 1 | 20170101 | Facebook HQ central |
| 0 | data/networks/fb\_nw.json | 37.48635 | -122.150289 | 1 | 20170101 | Facebook HQ North West |

## Wigle Exported Data Jobs

- if you used the Wigle mobile app to collect data, it can be exported to a CSV
- export one CSV per location you've scanned
- then run `python cli_jobs.py wigle_export -j ../data/jobs/wigle_export.csv`
- this will generate JSON files in the `data/networks` directory unless otherwise specified

| run | filepath | path_out | comment |
|---|---|---|---|---|
1 | data/wigle\_export/your\_location/scan1.csv | data/networks/ | Your comment



## Arduino Jobs

- Finally, convert the JSON files in the networks directory into an Arduino sketch 
- use `1` process or `0` to skip
- the `location` field text will be displayed on the OLED screen


| run  | filepath | name  | location  | max_networks | rssi_min | rssi_max | channel_min | channel_max |
|---|---|---|---|---|---|---|---|---|---|
1 | data/networks/fb\_central.json | Facebook (C) | "Menlo Park, CA" | 15 | -95 | -25 | 0 | 12 |
0 | data/networks/fb\_ne.json | Facebook (NE) | "Menlo Park, CA" | 15 | -95 | -25 | 0 | 12 |


## iOS Jobs

- (in progress)

