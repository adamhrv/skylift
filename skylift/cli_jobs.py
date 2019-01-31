import os
from pathlib import Path
from os.path import join

import pandas as pd
import click

from app.models.data_types import WiFiNet
from app.utils.net_parser import NetParser
from app.utils import scan_utils, logger_utils, file_utils, wigle_utils, data_utils
from app.settings import app_cfg as cfg


# ------------------------------------------------------
# Click Group
# ------------------------------------------------------
@click.group(chain=True)
@click.option('-v', '--verbose', 'opt_verbosity', count=True, default=4, 
show_default=True,
help='Verbosity: -v DEBUG, -vv INFO, -vvv WARN, -vvvv ERROR, -vvvvv CRITICAL')
@click.pass_context
def cli(ctx, opt_verbosity):
  logger_utils.Logger.create(verbosity=opt_verbosity)
  ctx.obj['log'] = logger_utils.Logger.getLogger()


# ------------------------------------------------------
# Fetch Wigle data
# ------------------------------------------------------
@cli.command('wigle', short_help='Fetches Wigle data')
@click.option('-j', '--jobs', 'opt_fp_jobs',
  default=cfg.FP_JOBS_WIGLE,
  help='Job file (CSV)')
@click.option('--wigle_api_name', 'opt_api_name', envvar='WIGLE_API_NAME',
  help='Wigle API name (typically 35 chars)')
@click.option('--wigle_api_key', 'opt_api_key',  envvar='WIGLE_API_KEY', 
  help='Wigle API token (typically 35 chars)')
@click.option('-f', '--force', 'opt_force', is_flag=True,
  help='Force overwrite file')
@click.pass_context
def cmd_wigle(ctx, opt_fp_jobs, opt_api_name, opt_api_key, opt_force):
  """Batch Fetch Wigle data from CSV"""

  log = ctx.obj['log']
  net_parser = NetParser()
  wigle = wigle_utils.WigleAPI(opt_api_name, opt_api_key)
  
  log.info('opening: {}'.format(opt_fp_jobs))
  jobs = pd.read_csv(opt_fp_jobs)
  jobs['comment'] = jobs['comment'].fillna('')  # hidden or empty SSIDs
  jobs['comment'] = jobs['comment'].astype('str') 
  
  for i, job in jobs.iterrows():
    if int(job['run']) == 0:
      continue
    log.debug('Fetch: {filename}, {lat}, {lon}, {since}, {radius}'.format(**job))
    url = wigle.build_url(job['lat'], job['lon'], job['radius'], job['since'])
    log.debug(url)
    networks = wigle.fetch(url, job['lat'], job['lon'])
    networks = net_parser.sort_distance(networks, 'wigle')

    # add metadata to context
    meta = dict(job.copy())  # copy the job variables
    meta['type'] = 'wigle'  # add ios type
    data = {'meta': meta, 'networks': networks}
    fp_out = join(job['path_out'], '{}.json'.format(job['filename']))
    if Path(fp_out).exists() and not opt_force:
      log.error('file exists "{}". use "-f" to overwrite'.format(fp_out))
    else:
      file_utils.write_json(data, fp_out, minify=False)



# ------------------------------------------------------
# Process Local Wigle Data from App Scan
# ------------------------------------------------------
@cli.command('wigle_export', short_help='Processed Exported Wigle data')
@click.option('-j', '--jobs', 'opt_fp_jobs',
  default=cfg.FP_JOBS_WIGLE_EXPORT,
  help='Job file (CSV)')
@click.option('-f', '--force', 'opt_force', is_flag=True,
  help='Force overwrite file')
@click.pass_context
def cmd_wigle(ctx, opt_fp_jobs, opt_force):
  """Run Wigle Export Job CSV"""

  log = ctx.obj['log']
  net_parser = NetParser()

  df_jobs = pd.read_csv(opt_fp_jobs, skiprows=(0))
  df_jobs['comment'] = df_jobs['comment'].fillna('').astype('str')
  
  for i, job in df_jobs.iterrows():
    if int(job['run']) == 0:
      continue

    fp_wigle = join(job.path_in, f'{job.filename}.csv')
    networks = net_parser.wigle_export_to_networks(fp_wigle, job.path_out, job.comment)
    networks = net_parser.sort_distance(networks, 'wigle_export')

    # add metadata to context
    meta = dict(job.copy())  # copy the job variables
    meta['type'] = 'wigle_export'  # add ios type
    data = {'meta': meta, 'networks': networks}
    fp_out = join(job['path_out'], '{}.json'.format(job.filename))
    if Path(fp_out).exists() and not opt_force:
      log.error('file exists "{}". use "-f" to overwrite'.format(fp_out))
    else:
      file_utils.write_json(data, fp_out, minify=False)



# --------------------------------------------------
# Convert iOS (txt) scan files to JSON
# --------------------------------------------------
@cli.command('ios')
@click.option('-j', '--jobs', 'opt_fp_jobs', required=True,
  default=cfg.FP_JOBS_IOS,
  help='Job file (CSV)')
@click.option('-o', '--output', 'opt_dir_out', 
  default=cfg.DIR_NETWORKS,
  help='Path to networks output directory')
@click.option('-f', '--force', 'opt_force', is_flag=True,
  help='Force overwrite file')
@click.pass_context
def cmd_ios(ctx, opt_fp_jobs, opt_dir_out, opt_force):
  """Convert iOS Airpot utility scan to networks JSON"""
  log = ctx.obj['log']
  parser = NetParser()

  # load data
  log.info('opening: {}'.format(opt_fp_jobs))
  jobs = pd.read_csv(opt_fp_jobs)

  for i, job in jobs.iterrows():
    # skip jobs if run == 0
    if int(job['run']) == 0:
      continue
    fp_ios = join(job['path_in'], '{}.csv'.format(job['filename']))
    networks = parser.ios_to_networks(fp_ios, job['lat'], job['lon'])
    meta = dict(job.copy())  # copy the job variables
    meta['type'] = 'ios'  # add ios type
    data = {'meta': meta, 'networks': networks}
    fp_out = join(job['path_out'], '{}.json'.format(job['filename']))
    if Path(fp_out).exists() and not opt_force:
      log.error('file exists "{}". use "-f" to overwrite'.format(fp_out))
    else:
      file_utils.write_json(data, fp_out, minify=False)



# --------------------------------------------------
# Convert JSON network files to Arduino headers (.h)
# --------------------------------------------------
@cli.command('arduino')
@click.option('-j', '--jobs', 'opt_fp_jobs', required=True,
  default=cfg.FP_JOBS_ARDUINO,
  help='Job file (CSV)')
@click.option('--sketch', 'opt_dir_sketch', default=None,
  help='Path to location summary file (Arduino .h')
@click.option('-f', '--force', 'opt_force', is_flag=True,
  help='Force overwrite file')
@click.pass_context
def cmd_arduino(ctx, opt_fp_jobs, opt_dir_sketch, opt_force):
  """Converts JSON to arduino .h file"""
  log = ctx.obj['log']
  parser = NetParser()

  # TODO: add network deduplication
  # TODO advanced: add kmeans cluster selection based on distances

  # load data
  log.info('opening: {}'.format(opt_fp_jobs))
  jobs = pd.read_csv(opt_fp_jobs)
  locations = []
  for i, job in jobs.iterrows():
    if int(job['run']) == 0:
      continue
    fp_location = join(job['path_in'], '{}.json'.format(job['filename']))
    log.debug(fp_location)
    location = file_utils.load_json(fp_location)
    n = location['networks']
    n = parser.filter_rssi(n, job['rssi_min'], job['rssi_max'])
    n = parser.filter_channel(n, job['channel_min'], job['channel_max'])
    n = parser.sort_distance(n, location['meta']['type'])
    n = n[:int(job['max_networks'])]
    location['networks'] = n
    locations.append(location)
    # format for arduino
    ino_txt = parser.networks_to_arduino(location, job['name'], job['location'])

    # write data to .h file
    if opt_dir_sketch:
      fp_out = join(opt_dir_sketch, 'networks', '{}.h'.format(job['filename']))
    else:
      fp_out = join(job['path_out'], '{}.h'.format(job['filename']))
    if Path(fp_out).exists() and not opt_force:
      log.error('file exists "{}". use "-f" to overwrite'.format(fp_out))
    else:
      file_utils.write_text(ino_txt, fp_out)

  # create network summary file
  location_summary_txt = parser.summarize_locations(locations)
  if opt_dir_sketch:
    fp_out = join(opt_dir_sketch, 'networks.h')
  else:
    fp_out = join(cfg.DIR_ARDUINO, 'networks.h')
  file_utils.write_text(location_summary_txt, fp_out)


# ------------------------------------------------------
# Entrypoint
# ------------------------------------------------------
if __name__ == '__main__':
  cli(obj={})
