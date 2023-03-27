#############################################################################
#
# SkyLift
# MIT License
# Copyright (c) 2016-2022 Adam Harvey
# https://github.com/adamhrv/skylift
#
#############################################################################


import click

from src.settings.app_cfg import INO_TEMPLATES

@click.command()
@click.option('-i', '--input', 'opt_input', required=True,
  help='Path to networks JSON')
@click.option('--board', 'opt_board', 
  type=click.Choice(INO_TEMPLATES.keys()),
  show_default=True,
  default='esp32',
  help='Arduino template type')
@click.option('-o', '--output', 'opt_output', required=True,
  help='Output sketcch directory')
@click.option('--max-networks', 'opt_max_networks', 
  type=click.IntRange(1,30),
  default=10)
@click.option('--max-rssi', 'opt_max_rssi', default=-30)
@click.option('--min-rssi', 'opt_min_rssi', default=-100)
@click.option('-c', '--channel', 'opt_channels', multiple=True,
  default=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13],
  help='Use multiple channels to spread across spectrum')
@click.option('--dbm', 'opt_wifi_dbm', default=19.5, type=float)
@click.pass_context
def cli(ctx, opt_input, opt_board, opt_output, opt_max_networks,
  opt_max_rssi, opt_min_rssi, opt_channels, opt_wifi_dbm):
  """Creates new Arduino sketch from template"""
  
  from os.path import join
  from pathlib import Path
  import shutil
  from glob import iglob

  from src.settings.app_cfg import LOG
  from src.utils.file_utils import load_txt, write_txt, load_json
  from src.utils.file_utils import mkdirs
  from src.models.network import Network, Networks

  # user input error checks
  if not Path(opt_input).suffix.lower() == '.json':
    LOG.error('Only JSON input supported')
    return

  # copy template files
  mkdirs(opt_output)
  dp_src = INO_TEMPLATES[opt_board]
  for fp_src in iglob(join(INO_TEMPLATES[opt_board], '*')):
    fp_dst = join(opt_output, Path(fp_src).name)
    shutil.copyfile(fp_src, fp_dst)
  fp_src = join(opt_output, f'{Path(dp_src).name}.ino')
  fp_dst = join(opt_output, f'{Path(opt_output).name}.ino')
  shutil.move(fp_src, fp_dst)

  # load networks
  networks = load_json(opt_input, data_class=Networks)
  wifi_nets = networks.get_networks(min_rssi=opt_min_rssi, 
    max_rssi=opt_max_rssi, 
    max_networks=opt_max_networks)

  # read template files
  t = load_txt(fp_dst, as_list=False)

  # replace templated tags
  def insert_template(src, tag, payload):
    tag_start, tag_end = f'// {{{{TEMPLATE:{tag}', f'// TEMPLATE:{tag}}}}}'
    idx_start = t.index(tag_start)
    idx_end = t.index(tag_end) + len(tag_end)
    return src[:idx_start] + str(payload) + src[idx_end:]

  # generate sketch data for template inserts
  templates = {}

  # header
  templates['HEADER'] = f'// Auto-generated from: {networks.filename}'

  # number networks
  templates['NN'] = f'#define NN {len(wifi_nets)}'

  # number channels
  templates['N_CHANNELS'] = f'#define N_CHANNELS {len(opt_channels)}'
  channels_str = ", ".join(list(map(str, opt_channels)))
  templates['CHANNELS'] = f'byte channels[N_CHANNELS] = {{{channels_str}}};'

  # esp32|82666
  if opt_board == 'esp32':
    templates['ESP'] = "#define ESP32 1"
  elif opt_board == 'esp8266':
    templates['ESP'] = "#define ESP8266 1"

  # ssids, bssids, channels, dbm_levels
  ssids = ['char* ssids[NN] = {']
  bssids = ['byte bssids[NN][6] = {']
  ssid_lengths = ['uint8_t ssid_lengths[NN] = {']
  for wifi_net in wifi_nets:
    ssids.append(f'\t"{wifi_net.ssid}", ')
    ssid_lengths.append(f'\t{len(wifi_net.ssid)}, ')
    bssids.append(f'\t{wifi_net.bssid_as_hex_list_ino()}, ')
  ssids.append('};')
  bssids.append('};')
  ssid_lengths.append('};')
  templates['SSIDS'] = '\n'.join(ssids)
  templates['SSID_LENGTHS'] = '\n'.join(ssid_lengths)
  templates['BSSIDS'] = '\n'.join(bssids)

  import random
  opts_channels = [1, 6, 11]
  use_channels = sorted([random.choice(opts_channels) for x in range(len(wifi_nets))])
  channels = ['byte channels[NN] = {']
  for c in use_channels:
    channels.append(f'\t{c},')
  channels.append('};')

  # wifi power
  templates['WIFI_POWER_DBM'] = f'float WIFI_POWER_DBM = {opt_wifi_dbm};  // ESP8266 only'

  # hidden ssids
  opt_hidden_ssids = False
  if opt_hidden_ssids:
    templates['HIDDEN_SSIDS'] = "#define USE_HIDDEN_SSIDS"
  else:
    templates['HIDDEN_SSIDS'] = "// #define USE_HIDDEN_SSIDS"

  for tag, payload in templates.items():
    t = insert_template(t, tag, payload)

  # write sketch data
  write_txt(fp_dst, t)


  LOG.info(f'Created "{opt_output}" with {len(wifi_nets)} wifi networks')