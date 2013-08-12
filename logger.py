#!/usr/bin/python
import serial, time
from datetime import datetime, timedelta

serialfile = "/dev/ttyUSB0"
logfile = 'data.dat'

if __name__ == '__main__':
    with open(logfile, 'a') as f:
        s = serial.Serial(serialfile, baudrate=9600)
        while True:
            tin = time.time()
            raw = s.readline().decode('utf-8')
            data = raw.rstrip('\r\n')
            tdiff = time.time() - tin
            try:
                value = float(data)
                print('{},{}'.format(
                        datetime.now() - timedelta(seconds=tdiff)/2.0,
                        value), file=f)
                f.flush()
            except:
                print("couldn't convert value {}".format(data), data)
                pass


