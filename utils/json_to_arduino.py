#!/usr/bin/env python
#coding=utf-8

import sys, os
from os.path import basename, join
import requests
import json
import argparse
from random import randint

'''
S K Y L I F T
https://github.com/adamhrv/skylift

WiFi utility script

- Converts JSON to Arduino code
'''

'''
Example:
python utils/json_to_arduino.py -i data/ios/your_scan.json -o data/ino/your_scan.txt
'''


def parse(args):
    bssids = []
    ssids = []

    # Load saved JSON from Wigle API
    with open(args.input, 'r') as f:
        networks = json.load(f)
    
    for n in networks:
        bssids.append(n['bssid'])
        ssids.append(n['ssid'])

    nmax = min(args.max_nets,len(ssids))

    bn = basename(args.input)
    fn, ext = os.path.splitext(bn)

    if args.output is None:
        output_fn = join('ino',fn + '.txt')
    else:
        output_fn = args.output

    with open(output_fn,'w') as f:

        f.write('#define nn {}\n\n'.format(nmax))
        f.write('char* ssids_str[nn] = {\n')

        # ssids as string
        for i,d in enumerate(bssids[:nmax]):
            d = '\t"'+d+'"'
            if i < len(bssids[:nmax])-1:
                d += ','
            f.write(d+'\n')
        f.write('};\n\n')

        # bssids as array
        f.write('byte bssids[nn][6] = {\n')
        for i,d in enumerate(bssids[:nmax]):
            d = [('0x'+e) for e in d.split(':')]
            d = ', '.join(d)
            d = '\t{'+d+'}'
            if i < len(bssids[:nmax])-1:
                d += ','
            f.write('{}\n'.format(d))
        f.write('};\n\n')

        # ssids as string
        f.write('char* ssids[nn] = {\n')
        for i,d in enumerate(ssids[:nmax]):
            if d is None:
                d = ''
            d = '\t"'+d+'"'
            if i < len(bssids[:nmax])-1:
                d += ','
        
            # encoding=utf8  
            #d = u''+d
            #d = unicode(d, 'utf-8')
            #d = unicodedata.normalize('NFKD', d).encode('ascii','ignore')
            d = d.encode('ascii', 'ignore').decode('ascii')
            f.write('{}\n'.format(d))
        f.write('};\n\n')

        # Add channels
        f.write('#ifdef USE_SCREEN\n')
        channels = ', '.join([ '6' for c in range(nmax)])
        channels = "byte channels[nn] = {"+channels+"};"
        f.write(channels+'\n')
        f.write('#else\n')
        channels = ', '.join([str(randint(0,12)) for b in range(nmax)])
        channels = "byte channels[nn] = {"+channels+"};"
        f.write(channels+'\n')
        f.write('#endif\n')

        print '[+] Saved {} networks from {} to {}'.format(nmax,args.input, output_fn)



def main(args):

    files = []
    if os.path.isdir(args.input):
        files = [join(args.input,f) for f in os.listdir(args.input) if f.endswith('.json')]
    else:
        files.append(args.input)

    for f in files:
        print '[+] Parsing "{}"'.format(f) 
        args.input = f
        parse(args)

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-i','--input',required=True,help='Input JSON file')
    parser.add_argument('-m','--max-nets',default=24,type=int,help='Max networks to include in Arduino file')
    parser.add_argument('-o','--output', help='Output file')
    args = parser.parse_args()

    main(args)
