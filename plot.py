#!/usr/bin/env python2
from pylab import *
import csv, sys
from datetime import datetime
from scipy.signal import gaussian

if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        r = csv.reader(f)
        dates, data = [], []
        for line in r:
            dates.append(datetime.strptime(line[0],
                                           "%Y-%m-%d %H:%M:%S.%f"))
            data.append(float(line[1]))

    data = array(data) / 0.000238 * 2e-4
    plot_date(dates, data, '.')
    nwin = 120;
    plot_date(dates, convolve(pad(data, nwin/2, mode='reflect'), 
        (lambda x:x/sum(x)) (gaussian(nwin, nwin/6.0)),
        mode='valid')[:len(dates)], 'r-', lw=3)
    ylabel(r'Dust concentration [$\mu g / m^3$]')
    yscale('log')
    ylim([1, None])
    grid()
    show()
