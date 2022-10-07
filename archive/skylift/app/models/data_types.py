from app.utils import geo_utils

class WiFiNet:

  def __init__(self, ssid, bssid, channel, rssi, 
    lat=0.0, lon=0.0, vendor=None, qos=0, lat_target=None, lon_target=None):
    self.ssid = ssid  # (str) 
    self.bssid = bssid  # (str) 00:11:22:33
    self.channel = int(channel)  # wifi channel
    self.rssi = int(rssi) if rssi is not None else rssi  # AP RSSI (not ESP8266 TX power)
    self.qos = qos  # quality of service (only for Wigle)
    self.lat = lat  # actual lat of wifi network
    self.lon = lon  # actual lon of wifi network
    self.vendor = vendor  # MAC vendor
    # target used for Wigle only
    self.lat_target = lat if not lat_target else lat_target
    self.lon_target = lon if not lon_target else lon_target

    p_targ = (self.lat_target, self.lon_target)
    p_actual = (self.lat, self.lon)
    p_y = (self.lat, self.lon_target)  # vary latitude
    p_x = (self.lat_target, self.lon)  # vary longitude
    self.distance_x = geo_utils.get_geo_distance(p_targ, p_x, signed=True)
    self.distance_y = geo_utils.get_geo_distance(p_targ, p_y, signed=True)
    self.distance_xy = geo_utils.get_geo_distance(p_targ, p_actual, signed=True)
    #network['distance_xy'] = math.sqrt( math.pow(x,2) + math.pow(y,2) )  # check if equal?

  def serialize(self):
    return {'ssid':self.ssid,
            'bssid':self.bssid,
            'channel':self.channel,
            'rssi':self.rssi,
            'qos':self.qos,
            'lat':self.lat,
            'lon':self.lon,
            'distance_x':self.distance_x,
            'distance_y':self.distance_y,
            'distance_xy':self.distance_xy,
            }

  def __repr__(self):
    return str(self.serialize())