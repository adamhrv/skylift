"""Utilities for accessing router vendor"""

from app.settings import app_cfg as cfg
from app.utils import file_utils


class MACVendors:

  def __init__(self):
    """Create vendor list"""
    self.vendors = {}
    
    vendor_list = file_utils.load_text(cfg.FP_MAC_VENDOR)

    for vendor in vendor_list:
      if not vendor or vendor[0] == '#' or vendor[0] == '\n':
        continue 
      vendor = vendor.split('\t')
      name = vendor[1].strip()
      full_name = name if len(vendor) == 2 else vendor[2].strip()
      self.vendors[vendor[0]] = {'name': name, 'name_full': full_name}


  def vendor(self, mac):
    """Returns name of the vendor"""
    # use only first 6 HEX chars
    vendor = self.vendors.get(mac[:8], None)
    if vendor:
      return vendor.get('name_full', None)
    else:
      return None