#!/usr/bin/env python
#coding=utf-8

import datetime
import os
import subprocess
import sys
import time
import signal
import RPi.GPIO as GPIO	

'''
S K Y L I F T
https://github.com/adamhrv/skylift

WiFi utility script
'''
  

def isMonitorModeEnabled():
	t = subprocess.Popen(args=["ifconfig"],stdout=subprocess.PIPE)
	output = t.communicate()[0]
	if "mon0" in output:
		return True
	else:
		return False

def enableMonitorMode(interface='wlan0',channel=11):
	airmon_args = ['airmon-ng','start',interface,str(channel)]
	ret  = None
	t = subprocess.Popen(args=airmon_args,stdout=subprocess.PIPE)
	t.wait()
	print "Monitor Mode Enabled!"

def disableMonitorMode():
	airmon_args = ['airmon-ng','stop','mon0']
	ret  = None
	t = subprocess.Popen(args=airmon_args,stdout=subprocess.PIPE)
	t.wait()
	print "Monitor Mode Disabled!"

def generateBeacons(channel=11):
	mdk3_args = [('mdk3 mon0 b -c %d -h %d -s 100')%(channel,channel)]
	ret  = None
	p = subprocess.Popen(args=mdk3_args	,shell=True, preexec_fn=os.setsid)

def generateProbes(channel=11):
	arg = 'mdk3 mon0 p -t %d -e Test 100'%(channel)
	print arg
	mdk3_args = [arg]

	ret  = None
	p = subprocess.Popen(args=mdk3_args,shell=True, preexec_fn=os.setsid)

def killMDK3():
	subprocess.Popen("killall mdk3", shell=True).wait()
	time.sleep(0.2)
	print "Killed mdk3!"


def updateMode(currentMode,channel):
	if currentMode == "Router":
		killMDK3()
		generateProbes(channel)
		print('Mode : Station')
		time.sleep(0.2)
	else:
		killMDK3()
		generateBeacons(channel)
		print('Mode : Router')
		time.sleep(0.2)

def updateChannel(c):
	killMDK3()
	time.sleep(0.2)
	disableMonitorMode()
	time.sleep(0.2)
	enableMonitorMode(channel=c)

if __name__ == '__main__':
	if isMonitorModeEnabled():
			print "Monitor mode already enabled"
	else:
		enableMonitorMode()
		# print "Monitor mode enabled"

# 	generateProbes()
# 	time.sleep(5)
# 	killMDK3()

	# generateProbes()

	#### NEW  ###	
	MODE_PIN  = 18
	CHANNEL_PIN  = 23
	channels = [1,6,11]
	c_idx =1
	
	modes = ["Router","Station"]
	m_idx = 1
	
	GPIO.setmode(GPIO.BCM)
	
	GPIO.setup(MODE_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
	GPIO.setup(CHANNEL_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

	while True:
		try:
			channel_up_input = GPIO.input(CHANNEL_PIN)
			if channel_up_input == False:

				c_idx = (c_idx + 1) if c_idx < len(channels) -1  else 0
				print "Switching to channel %d "%(channels[c_idx])
				updateChannel(channels[c_idx])
				updateMode(modes[m_idx],channels[c_idx])

			mode_input = GPIO.input(MODE_PIN)
			if mode_input == False:
				time.sleep(0.4)
				if m_idx:
					m_idx = 0
				else:
					m_idx = 1

				updateMode(modes[m_idx],channels[c_idx])
		except KeyboardInterrupt:
			print 'Interrupted'
			try:
				killMDK3()
				sys.exit(0)
			except SystemExit:
				killMDK3()
				os._exit(0)

		

