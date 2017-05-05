#!/usr/bin/env python
#coding=utf-8

import sys  
import os,sys
from os.path import join
import csv
import json
import argparse

'''
S K Y L I F T
https://github.com/adamhrv/skylift

WiFi utility script

- Reads in data from an iOS WiFi scan
- Outputs JSON formatted data
- Run the "json_to_arduino.py" script on this output to create Arduino code
- Assumes the first two lines are header information
'''

'''
Example:
python utils/ios_to_json.py -i data/ios/your_scan.txt -o data/json/your_scan.json
'''

def parse(args):
    
    # Load CSV
    scanned = []
    with open(args.input, 'rb') as f:
        csvs = csv.reader(f,skipinitialspace=True)
        start_at = 2
        for c in csvs:
            if c is not None and len(c) > 1:
                scanned.append(c)

    scanned = scanned[start_at:] #skip first x lines
    networks = []
    network_keys = []

    # create list with basic network information
    for ssid,bssid,rssi,channel,time in scanned:
    
        if ssid not in network_keys:
            network_keys.append(ssid)
            
            # filter threshold
            if args.threshold is not None:
                t = int(args.threshold)
                if int(rssi) < t:
                    continue
            
            # filter channel
            c = int(channel)
            # min
            if args.min_channel is not None:
                cmin = int(args.min_channel)
                if c < cmin:
                    continue
            # max
            if args.max_channel is not None:
                cmax = int(args.max_channel)
                if c > cmax:
                    continue

            networks.append({'ssid':ssid, 'bssid':bssid, 'channel':channel})

    # save list to JSON file
    if args.output is None:
        bn = os.path.basename(args.input)
        fn, ext = os.path.splitext(bn)
        output_fn = join('json',fn + '.json')
    else:
        output_fn = args.output

    with open(output_fn, 'w') as f:
        json.dump(networks, f, sort_keys = True, indent = 4, ensure_ascii=True)

    print '[+] Saved {} networks from iOS scan "{}"" to "{}"'.format(len(networks),args.input, output_fn)

def main(args):

    files = []
    if os.path.isdir(args.input):
        files = [join(args.input,f) for f in os.listdir(args.input) if f.endswith('.txt')]
    else:
        files.append(args.input)

    for f in files:
        print '[+] Parsing "{}"'.format(f) 
        args.input = f
        parse(args)



if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-i','--input',required=True,help='iOS Scan')
    parser.add_argument('-o','--output', required=False, help='Output file')
    parser.add_argument('-t','--threshold',default=-90,type=int,help='Minimum RSSI value')
    parser.add_argument('--min-channel',default=None,type=int,help='Minimum channel')
    parser.add_argument('--max-channel',default=None,type=int,help='Maximum channel')
    args = parser.parse_args()

    main(args)
