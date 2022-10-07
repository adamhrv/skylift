# ---------------------------------------------------------------------------
# Not yet implemented
# TODO: add OSX, Linux, Windows and instructions for iOS scanning
# ---------------------------------------------------------------------------

import os

import click

from app.models.data_types import WiFiNet
from app.utils.net_parser import NetParser
from app.utils import scan_utils, logger_utils, file_utils

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


# ------------------------------------------------------
# Scan
# ------------------------------------------------------
@cli.command('scan')
@click.option('-o', '--output', 'opt_fp_out', required=True)
@click.option('--os', 'opt_os', default='linux',
  type=click.Choice(['mac', 'linux', 'windows']))
@click.pass_context
def scan(ctx, opt_fp_out, opt_os):
  """Scan WiFi networks using local system"""
  # TODO: auto detect OS
  log = logger_utils.Logger.getLogger()
  log.error('not yet implemented')
  return
  
  scanner = scan_utils.Scanner()
  if opt_os == 'mac':
    scanner.osx()
  else:
    log.error('"{}" scan not yet implemented'.format(opt_os))


if __name__ == '__main__':
  cli()
