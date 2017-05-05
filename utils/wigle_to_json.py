#!/usr/bin/env python
#coding=utf-8

import sys, os
import requests
import json
import argparse


'''
S K Y L I F T
https://github.com/adamhrv/skylift

Wigle API for SkyLift

- Register at Wigle.net and obtain your API Key and Token first
- Loads data for Wigle API
- Outputs JSON formatted data
- Run the "json_to_arduino.py" script on this output to create Arduino code
'''

'''
Example:
python wigle_to_json.py --lat 37.484581 --lon -122.147921 --radius 3 -o facebook_r3.json
'''



'''
Get Lat/Lon here: 
http://www.latlong.net/convert-address-to-lat-long.html


'''

'''
Data returned from Wigle API:

{
    "bcninterval": 0, 
    "channel": 149, 
    "comment": null, 
    "dhcp": "?", 
    "firsttime": "2016-11-14T21:25:52.000Z", 
    "freenet": "?", 
    "lasttime": "2017-03-23T21:09:00.000Z", 
    "lastupdt": "2017-03-23T21:09:17.000Z", 
    "name": null, 
    "netid": "F0:99:BF:0C:CF:BD", 
    "paynet": "?", 
    "qos": 4, 
    "ssid": "SATMAP", 
    "transid": "20161115-00541", 
    "trilat": 38.89842224, 
    "trilong": -77.03987122, 
    "type": "infra", 
    "userfound": false, 
    "wep": "2"
}
'''

def main(args):

    # build URL
    lat = args.lat
    lon = args.lon
    radius_inc_lat = 0.000944 #multiply radius by this amount in lat direction
    radius_inc_lon = 0.001148 #multiply radius by this amount in lon direction
    r = args.radius
    lat_range = (lat - (radius_inc_lat*r), lat + (radius_inc_lat*r))
    lon_range = (lon - (radius_inc_lon*r), lon + (radius_inc_lon*r))

    url = 'https://api.wigle.net/api/v2/network/search?'
    url +='onlymine=false&'
    lat1 = args.lat - args.radius
    url += 'latrange1=' + str(lat_range[0]) + '&'
    url += 'latrange2=' + str(lat_range[1]) + '&'
    url += 'longrange1=' + str(lon_range[0]) + '&'
    url += 'longrange2=' + str(lon_range[1]) + '&'
    url += 'freenet=false&'
    url += 'paynet=false'

    print '[+] Requesting networks'
    wigle_networks = requests.get(url, headers={'Authentication':'Basic'},auth = (args.api_name,args.api_token))
    wigle_networks = wigle_networks.json()['results']

    networks = []
    for d in wigle_networks:
        d = {'ssid':d['ssid'], 'bssid':d['netid'], 'channel':d['channel']}
        networks.append(d)
    
    output_fn = join('json',args.output)

    with open(output_fn, 'w') as f:
        json.dump(networks, f,sort_keys = True, indent = 4)

    print '[+] Found {} networks on Wigle for lat/lon: {},{}'.format(len(networks),args.lat, args.lon)

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-o','--output', required=True, help='Output JSON file')
    parser.add_argument('--api-name', required=True, help='Wigle API Name')
    parser.add_argument('--api-token', required=True, help='Wigle API Key')
    parser.add_argument('--lat', default=37.422000, type=float, required=False, help='latitude') # Mountain view
    parser.add_argument('--lon', default=-122.084057, type=float, required=False, help='longitude') # Mountain view
    parser.add_argument('-t','--last-update',default='20160601',help="Filter points by yyyyMMdd")
    parser.add_argument('--radius', default=1, type=int, required=False, help='radius scale, default=1 (roughly 100m)')
    #parser.add_argument('--freenet', default=False, help='Wigle API Key')
    #parser.add_argument('--paynet', default=False, help='Wigle API Key')
    #parser.add_argument('--onlymine', default=False, help='Only mine')
    parser.add_argument('-f', '--overwrite', action='store_true', help='overwrite existing files')
    args = parser.parse_args()

    main(args)
