#############################################################################
#
# SkyLift
# MIT License
# Copyright (c) 2016-2022 Adam Harvey
# https://github.com/adamhrv/skylift
#
#############################################################################


import click

@click.command()
@click.option('-i', '--input', 'opt_input', required=True,
  help='Network scan CSV')
@click.option('-o', '--output', 'opt_output',
  help='Network output JSON')
@click.pass_context
def cli(ctx, opt_input, opt_output):
  """Convert WiFi/BT network can to Networks JSON"""

  from pathlib import Path
  import json

  import pandas as pd
  
  from src.settings.app_cfg import LOG, SCAN_TYPE_LKUP, ScanType
  from src.settings.app_cfg import DTYPES_WIGLE_CSV, DTYPES_IOS_CSV
  from src.settings.app_cfg import WIGLE_COL_MAPPINGS, IOS_COL_MAPPINGS 
  from src.models.network import Network, Networks
  from src.utils.file_utils import write_json

  # user input error checks
  if not Path(opt_input).suffix.lower() == '.csv':
    LOG.error('Only CSV supported')
    return

  opt_output = opt_output if opt_output else Path(opt_input).with_suffix('.json')

  # read header to auto-infer scan type
  h1 = list(pd.read_csv(opt_input, nrows=1).keys())[0].lower()
  h1 = h1.split('-')[0] if '-' in h1 else h1
  scan_data = {}


  # Wigle scan
  if SCAN_TYPE_LKUP[h1] == ScanType.WIGLE:

    # read wigle scan
    dtypes = DTYPES_WIGLE_CSV
    df = pd.read_csv(opt_input, header=1, usecols=dtypes.keys(), dtype=dtypes)
    df.rename(columns=WIGLE_COL_MAPPINGS, inplace=True)
    df = df.fillna(value={'ssid':''})
    df = df.sort_values(by=['rssi'], ascending=False).drop_duplicates('bssid')

    # group by wifi, ble (ignore non-ble)
    df_wifi = df[df['Type'] == 'WIFI'].drop('Type', axis=1)
    df_ble = df[df['Type'] == 'BLE'].drop('Type', axis=1)
    
    # convert to dataclass
    ls_wifi = [Network(**d) for d in df_wifi.to_dict('records')]
    ls_bt = [Network(**d) for d in df_ble.to_dict('records')]
    scan_data = {'wifi': ls_wifi, 'bt': ls_bt}

  # IOS scan
  elif SCAN_TYPE_LKUP[h1] == ScanType.IOS:

    # read ios csv scan
    df = pd.read_csv(opt_input, dtype=DTYPES_IOS_CSV)
    df = df.rename(columns=IOS_COL_MAPPINGS)
    df = df.sort_values(by=['rssi'], ascending=False).drop_duplicates('bssid')
    df = df.fillna(value={'ssid':''})

    # parse wifi networks into JSON
    ls_wifi = [Network(**d) for d in df.to_dict('records')]
    scan_data = {'filename': Path(opt_input).name, 'wifi': ls_wifi}

  # Not available
  else:
    LOG.debug(f'No type found for header col 1 {h1}')

  # write to json
  if scan_data:
    scan_data['filename'] = Path(opt_input).name
    nets = Networks(**scan_data)
    nets.model_post_init()
    write_json(opt_output, nets.dict(), minify=False)
    LOG.info(f'Wrote {len(nets.wifi)} WiFi networks, {len(nets.bt)} Bluetooth networks')
  else:
    LOG.info('No networks')