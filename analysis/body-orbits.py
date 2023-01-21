import sys
import numpy as np
import pandas as pd
from common import read_sat, MOON_A, L12_DST
from matplotlib import cm
from matplotlib import pyplot as plt

D = 3.844e8
d = 4.669e6

if __name__ == "__main__":
    filepath = sys.argv[1]
    print(filepath)
    t, x, y = read_sat(filepath)
    x: np.ndarray = x
    r_start = (x[0,:]**2 + y[0,:]**2)**0.5
    r_start_min = np.nanmin(r_start)
    r_start_max = np.nanmax(r_start)
    print("r_start_min:", r_start_min)
    print("r_start_max:", r_start_max)

    plt.figure()
    for i in range(x.shape[1]):
        plt.plot(x[:,i], y[:,i], '-', color=cm.brg((r_start[i] - r_start_min) / (r_start_max - r_start_min)))

    print(x.shape[1])
    ax = plt.gca()
    ax.scatter(-d, 0, s=120, label="Earth")
    ax.scatter(D -d, 0, s=100, label="Moon")
    ax.plot(MOON_A - L12_DST, 0, 'x', label='L1')
    ax.annotate("L1", (MOON_A - L12_DST, 0), textcoords="offset points", xytext=(0, 10), ha='center')
    ax.plot(MOON_A + L12_DST, 0, 'x', label="L2")
    ax.annotate("L2", (MOON_A + L12_DST, 0), textcoords="offset points", xytext=(0, 10), ha='center')
    plt.xlim(-6e8, 6e8)
    plt.ylim(-6e8, 6e8)
    plt.legend(loc="upper right")
    ax.set_aspect(1)
    plt.show()