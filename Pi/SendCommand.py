#!/usr/bin/python

import serial
import os
import sys

dev = "/dev/ttyACM0"
if not os.path.exists( dev ):
	dev = "/dev/ttyACM1"
if not os.path.exists( dev ):
	print( "Could not find a device to use" )
	sys.exit(1)
print( "Using device " + dev )
ser = serial.Serial( dev )

cmd = ' '.join( sys.argv[ 1 : ] )

print( "Sending: " + cmd )
ser.write( cmd + "\n" )
out = ser.readline()
print( "> " + out )
print( "Done." )

