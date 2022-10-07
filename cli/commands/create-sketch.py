#############################################################################
#
# SkyLift
# MIT License
# Copyright (c) 2016-2022 Adam Harvey
# https://github.com/adamhrv/skylift
#
#############################################################################


import click

from app.settings.app_cfg import INO_TEMPLATES

@click.command()
@click.option('-i', '--input', 'opt_input', required=True,
  help='Path to networks JSON')
@click.option('-t', '--type', 'opt_type', 
  type=click.Choice(INO_TEMPLATES.keys()),
  show_default=True,
  default='esp32',
  help='Arduino template type')
@click.option('-o', '--output', 'opt_output', required=True,
  help='Output sketcch directory')
@click.option('--max-networks', 'opt_max_networks', 
  type=click.IntRange(1,20),
  default=10)
@click.option('--max-rssi', 'opt_max_rssi', default=-30)
@click.option('--min-rssi', 'opt_min_rssi', default=-100)
@click.pass_context
def cli(ctx, opt_input, opt_type, opt_output, opt_max_networks,
  opt_max_rssi, opt_min_rssi):
  """Creates new Arduino sketch from template"""
  
  from os.path import join
  from pathlib import Path
  import shutil
  from glob import iglob

  from app.settings.app_cfg import LOG
  from app.utils.file_utils import load_txt, write_txt, load_json
  from app.utils.file_utils import mkdirs
  from app.models.network import Network, Networks

  # copy template files
  mkdirs(opt_output)
  dp_src = INO_TEMPLATES[opt_type]
  for fp_src in iglob(join(INO_TEMPLATES[opt_type], '*')):
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
  template_inserts = {}

  # header
  template_inserts['HEADER'] = f'// Auto-generated from: {networks.filename}'

  # number networks
  template_inserts['NN'] = f'#define NN {len(wifi_nets)}'

  # esp32|82666
  if opt_type == 'esp32':
    template_inserts['ESP'] = "#define ESP32 1"
  elif opt_type == 'esp8266':
    template_inserts['ESP'] = "#define ESP8266 1"

  # ssids, bssids, channels, dbm_levels
  ssids = ['char* ssids[NN] = {']
  bssids = ['byte bssids[NN][6] = {']
  for wifi_net in wifi_nets:
    ssids.append(f'\t"{wifi_net.ssid}",')
    bssids.append(f'\t{wifi_net.bssid_as_hex_list_ino()},')
  ssids.append('};')
  bssids.append('};')
  template_inserts['SSIDS'] = '\n'.join(ssids)
  template_inserts['BSSIDS'] = '\n'.join(bssids)

  import random
  opts_channels = [1, 6, 11]
  use_channels = sorted([random.choice(opts_channels) for x in range(len(wifi_nets))])
  channels = ['byte channels[NN] = {']
  for c in use_channels:
    channels.append(f'\t{c},')
  channels.append('};')
  template_inserts['CHANNELS'] = '\n'.join(channels)

  # hidden ssids
  opt_hidden_ssids = False
  if opt_hidden_ssids:
    template_inserts['HIDDEN_SSIDS'] = "#define USE_HIDDEN_SSIDS"
  else:
    template_inserts['HIDDEN_SSIDS'] = "// #define USE_HIDDEN_SSIDS"

  for tag, payload in template_inserts.items():
    t = insert_template(t, tag, payload)

  # write sketch data
  write_txt(fp_dst, t)


  LOG.info(f'Created "{opt_output}"')