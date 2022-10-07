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
@click.option('-o', '--output', 'opt_output', required=True,
  help='Network output JSON')
@click.pass_context
def cli(ctx, opt_input, opt_output):
  """Convert WiFi/BT network can to Networks JSON"""

  from dataclasses import asdict
  from pathlib import Path
  
  import pandas as pd
  from dacite import from_dict
  
  from app.settings.app_cfg import LOG, SCAN_TYPE_LKUP, ScanType
  from app.settings.app_cfg import DTYPES_WIGLE_CSV, DTYPES_IOS_CSV
  from app.settings.app_cfg import WIGLE_COL_MAPPINGS, IOS_COL_MAPPINGS 
  from app.models.network import Network, Networks
  from app.utils.file_utils import write_json

  # user input error checks
  if not Path(opt_input).suffix.lower() == '.csv':
    LOG.error('Only CSV supported')
    return
  if not Path(opt_output).suffix.lower() == '.json':
    LOG.error('Only JSON output')
    return

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

    # group by wifi, ble (ignore non-ble)
    df_wifi = df[df['Type'] == 'WIFI'].drop('Type', axis=1)
    df_ble = df[df['Type'] == 'BLE'].drop('Type', axis=1)
    
    # convert to dataclass
    ls_wifi = [from_dict(data_class=Network, data=d) for d in df_wifi.to_dict('records')]
    ls_bt = [from_dict(data_class=Network, data=d) for d in df_ble.to_dict('records')]
    scan_data = {'wifi': ls_wifi, 'bt': ls_bt}

  # IOS scan
  elif SCAN_TYPE_LKUP[h1] == ScanType.IOS:

    # read ios csv scan
    df = pd.read_csv(opt_input, dtype=DTYPES_IOS_CSV)
    df = df.rename(columns=IOS_COL_MAPPINGS)
    df = df.sort_values(by=['rssi'], ascending=False).drop_duplicates('bssid')
    df = df.fillna(value={'ssid':''})

    # parse wifi networks into JSON
    ls_wifi = [from_dict(data_class=Network, data=d) for d in df.to_dict('records')]
    scan_data = {'filename': Path(opt_input).name, 'wifi': ls_wifi}

  # Not available
  else:
    LOG.debug(f'No type found for header col 1 {h1}')


  # write to json
  if scan_data:
    scan_data['filename'] = Path(opt_input).name
    nets = from_dict(data_class=Networks, data=scan_data)
    write_json(opt_output, nets, minify=False)
    LOG.info(f'Wrote {nets.n_wifi} WiFi networks, {nets.n_bt} Bluetooth networks')
  else:
    LOG.info('No networks')