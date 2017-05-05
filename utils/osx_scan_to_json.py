#!/usr/bin/env python
#coding=utf-8

import sys, os
import json
import argparse
import subprocess
import plistlib

'''

https://github.com/adamhrv/skylift


TODO
- fix folder paths
- 
'''

'''
Create a shortcut to test with the "airport" app:
sudo ln -s /System/Library/PrivateFrameworks/Apple/80211.framework/Versions/Current/Resources/airport /usr/sbin/airport

Test it:
airport -s

Output as XML
airport -s -x > temp.xml 
'''

def main(argss):

    # TODO add check for airport scan app
    print '[+] Scanning for WiFi networks'

    airport = '/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport'
    t = subprocess.Popen(args=[airport,'-s','-x'],stdout=subprocess.PIPE)
    output = t.communicate()[0]
    scan_xml = plistlib.readPlistFromString(output)

    networks = []
    for network in scan_xml:
        # Ensure 2-digit hexidecimal
        bssid = ':'.join(map(lambda x: x if (len(x)==2) else ('0'+x), 
            network['BSSID'].split(':')))
        d = {'bssid':bssid,
            'ssid':network['SSID_STR'],
            'channel':network['CHANNEL']}
        networks.append(d)
    
    # TODO handle unicode network names
    output_fn = 'sample.json'
    with open(output_fn, 'w') as f:
            json.dump(networks, f, sort_keys = True, indent = 4, ensure_ascii=True)

    print '[+] Saved {} networks to "{}"'.format(len(networks),output_fn)


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-o','--output', required=True, help='Output JSON file')
    args = parser.parse_args()

    main(args)